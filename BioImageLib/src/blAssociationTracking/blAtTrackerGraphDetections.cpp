/// \file blAtTrackerGraphDetections.cpp
/// \brief blAtTrackerGraphDetections class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2013

#include "blAtTrackerGraphDetections.h"
#include "lemon/bellman_ford.h"
#include <algorithm>

using namespace std;

blAtTrackerGraphDetections::blAtTrackerGraphDetections(blAtCostInterface *costFunction, vector<string> framesFiles, vector<vector<vector<float> > > objects)
    :blAtTrackerInterface(costFunction, framesFiles, objects){
initParametersWithDefaultsValues();
}

blAtTrackerGraphDetections::blAtTrackerGraphDetections() : blAtTrackerInterface(){
    initParametersWithDefaultsValues();
}

blAtTrackerGraphDetections::~blAtTrackerGraphDetections(){

}

// ////////////////// parameters ////////////////////////
void blAtTrackerGraphDetections::initParametersWithDefaultsValues(){
    m_L = 140.0;
    m_jumpEpsilon = 0.01;
    m_coefIntegerConversion = 100;
    m_optimizer = "shortestPath";
    // required amount of flow from node s to node t
    m_requiredFlow = 100;
}

// ////////////// setters ///////////////////////////////////
void blAtTrackerGraphDetections::setMaxMove(float L){
    m_L = L;
}

void blAtTrackerGraphDetections::setJumpPenalty(float penalty){
    m_jumpEpsilon = penalty;
}

void blAtTrackerGraphDetections::setCoefIntegerConversion(float coef){
    m_coefIntegerConversion = coef;
}

void blAtTrackerGraphDetections::setOptimizerName(std::string name){
    m_optimizer = name;
}

void blAtTrackerGraphDetections::setRequiredFlow(int flow){
    m_requiredFlow = flow;
}

void blAtTrackerGraphDetections::setUseOptimizedGraph(int useOptimizedBuild){
    m_useOptimizedBuild = false;
    if (useOptimizedBuild > 0){
        m_useOptimizedBuild = true;
    }
}

// /////////////////////////////////////////////////////////
void blAtTrackerGraphDetections::run(){

    cout << "blAtTrackerGraphDetections m_L = " << m_L << endl;
    cout << "blAtTrackerGraphDetections::update begin" << endl;
    // Get parameters
    m_costFunction->setLParameterFromOptimizer(m_L);
    m_costFunction->setFramesPath(m_framesFiles);
    m_costFunction->setObjects(m_objects);

    if (m_optimizer == "minCostFlow"){
        cout << "blAtTrackerGraphDetections::update minCostFlow" << endl;
        updateMinCostFlow();
    }
    else if (m_optimizer == "shortestPath"){
        cout << "blAtTrackerGraphDetections::update shortestPath" << endl;
        updateShortestPath();
    }
    cout << "blAtTrackerGraphDetections::update end" << endl;
}

void blAtTrackerGraphDetections::updateShortestPath(){
    m_tracksAlreadyCalculated = true;
    cout << endl << "updateShortestPath -> Begining" << endl;
    // create the graph
    cout << " \t updateShortestPath -> create the graph" << endl;
    lemon::ListDigraph g;
    lemon::ListDigraph::ArcMap<int> cap(g);
    lemon::ListDigraph::ArcMap<int> cost(g);
    lemon::ListDigraph::Node source;
    lemon::ListDigraph::Node target;
    vector<lemon::ListDigraph::Arc> arcs;
    vector<vector<int> > nodesConnectedArcsIds;

    cout << " \t updateShortestPath -> create graph" << endl;
    if (m_useOptimizedBuild){
        buildGraphNegativeOptimized(g, cap, cost, source, target, arcs, nodesConnectedArcsIds);
    }
    else{
        buildGraphNegative(g, cap, cost, source, target, arcs, nodesConnectedArcsIds);
    }

    cout << "node num: " << g.maxNodeId() << endl;
    cout << "arcs num: " << arcs.size() << " = " << g.maxArcId()+1 << endl << endl;

    bool stop = false;
    //vector<blAtConnection*> connectionsInter;
    int iter = -1;
    while (!stop){
        iter++;
        if (iter%10 == 0){
            cout << "shortest path: Compute track " << iter << endl;
        }
        // shortest path
        //cout << " \t updateShortestPath -> shortest path instantiate" << endl;
        lemon::BellmanFord<lemon::ListDigraph, lemon::ListDigraph::ArcMap<int> > bf(g, cost);
        //cout << " \t updateShortestPath -> shortest path run" << endl;
        bf.run(source);
        //cout << " \t updateShortestPath -> shortest path, get path" << endl;
        lemon::Path<lemon::ListDigraph> p = bf.path(target);
        //cout << "curent path length = " << p.length() << endl;
        // get the nodes and arcs in the path

        if (!lemon::checkPath(g,p)){
            stop = true;
        }
        else{
            // create the path
            //connectionsInter.clear();
            blAtTrack *new_track = new blAtTrack();
            vector<int> new_track_cost;
            //cout << " \t updateShortestPath -> create the path" << endl;
            lemon::ListDigraph::Node curentNodeSource;
            lemon::ListDigraph::Node curentNodeTarget;


            lemon::Path<lemon::ListDigraph>::ArcIt it(p);

            int compt = 0;
            while (it != lemon::INVALID) {
                compt++;
                curentNodeSource = g.source(it);
                curentNodeTarget = g.target(it);



                int sourceId = g.id(curentNodeSource);
                int targetId = g.id(curentNodeTarget);

                if (sourceId > 1 && targetId > 1){
                    new_track->addConnection(new blAtConnection(m_nodesData.at(sourceId-2).state(),
                                                                  m_nodesData.at(targetId-2).state(),
                                                                  m_nodesData.at(sourceId-2).frameIdx(),
                                                                  m_nodesData.at(targetId-2).frameIdx() ));
                    new_track_cost.push_back(cost.operator [](it));
                }

                if (sourceId > 1){


                    int id = g.id(curentNodeSource);
                    for (unsigned int i = 0 ; i < nodesConnectedArcsIds[id].size() ; ++i){
                        lemon::ListDigraph::Arc arc = g.arcFromId(i);
                        if (g.valid(arc)){
                            g.erase(arc);
                        }
                    }

                }
                if (compt > 1){
                    g.erase(curentNodeSource);
                }
                ++it;
            }

            if (new_track->getTrackSize()<= 2){
                stop = true;
                delete new_track;
                break;
            }
            if (!stop){
                // create the new track
                m_tracks.push_back(new_track);
                m_tracksCosts.push_back(new_track_cost);
            }
        }
    }

    cout << "shortest path: founded " << m_tracks.size() << " tracks" << endl;
    cout << "shortest path: founded cost " << m_tracksCosts.size() << " tracks" << endl;
}

void blAtTrackerGraphDetections::updateMinCostFlow(){


    // create the graph
    lemon::ListDigraph g;
    lemon::ListDigraph::ArcMap<int> cap(g);
    lemon::ListDigraph::ArcMap<int> cost(g);
    lemon::ListDigraph::Node source;
    lemon::ListDigraph::Node target;
    vector<lemon::ListDigraph::Arc> arcs;
    vector< vector<int> > arcsInOutMap;
    buildGraphNegative(g, cap, cost, source, target, arcs, arcsInOutMap);

    cout << "arcs num: " << arcs.size() << " = " << arcsInOutMap.size() << endl;

    // find minimim cost flow
    lemon::CapacityScaling<lemon::ListDigraph> cs(g);

    // First run
    cs.upperMap(cap).costMap(cost).stSupply(source, target, m_requiredFlow).run();

    std::cout << "CapacityScaling, Total cost: " << cs.totalCost<double>() << std::endl;

    lemon::ListDigraph::ArcMap<int> flow(g);
    cs.flowMap(flow);

    /*
    // plot the graph
    digraphWriter(g).                  // write g to the standard output
            arcMap("capacity", cap).       // write cap into 'capacity'
            arcMap("cost", cost).          // write 'cost' for for arcs
            arcMap("flow", flow).          // write 'flow' for for arcs
            node("source", source).             // write s to 'source'
            node("target", target).             // write t to 'target'
            run();
*/
    // Get the list of keeped arcs
    cout << "Get the list of keeped arcs" << endl;
    vector<vector<int> > keepedArcsIO;
    for (unsigned int i = 0 ; i < arcs.size() ; ++i){
        if ( flow[arcs.at(i)] == 1 ){
            keepedArcsIO.push_back(arcsInOutMap.at(i));
        }
    }
    cout << "keeped size = " << keepedArcsIO.size() << endl;

    for (unsigned int i = 0 ; i < keepedArcsIO.size() ; ++i){
        if (keepedArcsIO[i][0] > 1 && keepedArcsIO[i][1] > 1){

            m_connections.push_back(new blAtConnection(m_nodesData.at(keepedArcsIO[i][0]-2).state(),
                                    m_nodesData.at(keepedArcsIO[i][1]-2).state(),
                    m_nodesData.at(keepedArcsIO[i][0]-2).frameIdx(),
                    m_nodesData.at(keepedArcsIO[i][1]-2).frameIdx() ));
        }
    }
    cout << "delete connections" << endl;
    //deleteInitialConnections();
    cout << "graph end update: founded connection = " << m_connections.size() << endl;
}

void blAtTrackerGraphDetections::buildGraphNegative(lemon::ListDigraph &g, lemon::ListDigraph::ArcMap<int> &cap, lemon::ListDigraph::ArcMap<int> &cost, lemon::ListDigraph::Node &source, lemon::ListDigraph::Node &target, vector<lemon::ListDigraph::Arc> &arcs){


    // 1- Create the object list
    m_nodesData.clear();
    int countor = 0;
    for (unsigned int frame = 0 ; frame < m_objects.size() ; ++frame){
        for (unsigned int objIdx = 0 ; objIdx < m_objects[frame].size() ; ++ objIdx){
            countor++;
            m_nodesData.push_back(blAtDetection(m_objects[frame][objIdx], frame, objIdx));
        }
    }

    // 2- Create the detection nodes
    // reserve size
    g.reserveNode(countor);
    // add nodes
    vector<lemon::ListDigraph::Node> nodes;
    source = g.addNode();
    target = g.addNode();
    nodes.push_back(source);
    nodes.push_back(target);
    for (unsigned int i = 0 ; i < m_nodesData.size() ; ++i){
        nodes.push_back(g.addNode());
    }

    // 3- create the arcs
    // 3.1- To source and target
    //vector<int> arcIOInter; arcIOInter.resize(2);
    lemon::ListDigraph::Arc arc;
    g.reserveArc(countor);
    for (unsigned int i = 0 ; i < nodes.size()-2 ; ++i){
        // link to source
        arc = g.addArc(source, nodes[i+2]); cap[arc] = 1; cost[arc] = 0;
        arcs.push_back(arc);
        // link to target
        arc = g.addArc(nodes[i+2], target); cap[arc] = 1; cost[arc] = 0;
        arcs.push_back(arc);

    }

    // 3.2- Between detections
    //cout << "m_frameNumber = " << m_frameNumber << endl;
    //cout << "objects size = " << m_objects.size() << ", " << m_objects[0].size() << endl;
    float curentCost;
    float maxCost, costLimit;
    if (m_costFunction->isProbability()){
        maxCost = 1.0;
        costLimit = 1.0;
    }
    else{
        maxCost = m_timeSpread*m_L*m_L;
        costLimit = m_L*m_L;
    }
    for (unsigned int t=0 ; t<m_frameNumber-1 ; ++t){
        cout << "ConnectionManager -> connect frame: " << t << endl;

        // Get the number of future frame to scan depending on the remaining number of frames
        int endlength = 1;
        if (t < m_frameNumber-1-m_timeSpread ){
            endlength = m_timeSpread;
        }
        else{
            endlength = m_frameNumber-1-t;
        }
        //cout << "endlength = " << endlength << endl;
        // Fill the graph by scanning the connectons

        for (unsigned int p = t+1 ; p <= t+endlength ; ++p){
            cout << "t = " << t << ", p = " << p << endl;
            cout << "t size = " << m_objects[t].size() << ", p size = " << m_objects[p].size() << endl;
            m_costFunction->loadDataCurentFrames(t, p);
            //cout << "blAtTrackerGraph finish load frames" << endl;
            for (unsigned int nt = 0 ; nt< m_objects[t].size() ; ++nt){
                //cout << "\t nt = " << nt << endl;
                for (unsigned int np = 0 ; np<m_objects[p].size(); ++np){
                    //cout << "\t \t np = " << np << endl;
                    blAtConnection *connection = new blAtConnection(m_objects[t][nt], m_objects[p][np], t, p);
                    //cout << "calculate cost" << endl;
                    curentCost = m_costFunction->calculateCost(connection);
                    delete connection;

                    if (curentCost < costLimit){ // (p-t)*m_L*m_L

                        // add arc to graph
                        int posObject1 = CalculateNodeIdx(t, nt);
                        int posObject2 = CalculateNodeIdx(p, np);
                        //if (t == 0){
                        //    cout << "add arc between " << posObject1+2 << ", and " << posObject2+2 << endl;
                        //}
                        arc = g.addArc( nodes[posObject1+2], nodes[posObject2+2] );
                        cap[arc] = 1;
                        if (p-t-1 > 0 ){
                            cost[arc] = int((curentCost/maxCost -1.0 - (p-t-1) +m_jumpEpsilon)*m_coefIntegerConversion);
                        }
                        else{
                            cost[arc] = int((curentCost/maxCost -1.0)*m_coefIntegerConversion);
                        }
                        arcs.push_back(arc);
                    }
                }
            }
        }
    }
}

void blAtTrackerGraphDetections::buildGraphNegativeOptimized(lemon::ListDigraph &g, lemon::ListDigraph::ArcMap<int> &cap, lemon::ListDigraph::ArcMap<int> &cost, lemon::ListDigraph::Node &source, lemon::ListDigraph::Node &target, vector<lemon::ListDigraph::Arc> &arcs, vector<vector<int> > &nodesConnectedArcsIds){


    // 0- Sort objects
    for (unsigned int frame = 0 ; frame < m_objects.size() ; ++frame){
        std::sort(m_objects[frame].begin(), m_objects[frame].end(), compareX);
    }

    // 1- Create the object list
    m_nodesData.clear();
    int countor = 0;
    for (unsigned int frame = 0 ; frame < m_objects.size() ; ++frame){
        for (unsigned int objIdx = 0 ; objIdx < m_objects[frame].size() ; ++ objIdx){
            countor++;
            m_nodesData.push_back(blAtDetection(m_objects[frame][objIdx], frame, objIdx));
        }
    }
    nodesConnectedArcsIds.resize(m_nodesData.size()+2);

    // 2- Create the detection nodes
    // reserve size
    g.reserveNode(countor+2);
    // add nodes
    vector<lemon::ListDigraph::Node> nodes; /// \todo use g.NodeFromId in stead of this compy ?
    source = g.addNode();
    target = g.addNode();
    nodes.push_back(source);
    nodes.push_back(target);
    for (unsigned int i = 0 ; i < m_nodesData.size() ; ++i){
        nodes.push_back(g.addNode());
    }

    // 3- create the arcs
    // 3.1- To source and target
    //vector<int> arcIOInter; arcIOInter.resize(2);
    lemon::ListDigraph::Arc arc;
    g.reserveArc(countor);
    int idArd;
    for (unsigned int i = 0 ; i < nodes.size()-2 ; ++i){
        // link to source
        arc = g.addArc(source, nodes[i+2]); cap[arc] = 1; cost[arc] = 0;
        arcs.push_back(arc);
        idArd= g.id(arc);
        nodesConnectedArcsIds[i+2].push_back(idArd);
        // link to target
        arc = g.addArc(nodes[i+2], target); cap[arc] = 1; cost[arc] = 0;
        arcs.push_back(arc);
        idArd= g.id(arc);
        nodesConnectedArcsIds[i+2].push_back(idArd);
    }

    // 3.2- Between detections
    //cout << "m_frameNumber = " << m_frameNumber << endl;
    //cout << "objects size = " << m_objects.size() << ", " << m_objects[0].size() << endl;
    float curentCost;
    float maxCost, costLimit;
    if (m_costFunction->isProbability()){
        maxCost = 1.0;
        costLimit = 1.0;
    }
    else{
        maxCost = m_L*m_L;
        costLimit = m_L*m_L;
    }
    int m_Lplus1 = m_L+1;
    int m_Lmenus1 = m_L-1;
    for (unsigned int t=0 ; t<m_frameNumber-1 ; ++t){
        cout << "ConnectionManager -> connect frame: " << t << endl;

        // Get the number of future frame to scan depending on the remaining number of frames
        int endlength = 1;
        if (t < m_frameNumber-1-m_timeSpread ){
            endlength = m_timeSpread;
        }
        else{
            endlength = m_frameNumber-1-t;
        }
        //cout << "endlength = " << endlength << endl;
        // Fill the graph by scanning the connectons

        for (unsigned int p = t+1 ; p <= t+endlength ; ++p){
            cout << "t = " << t << ", p = " << p << endl;
            cout << "t size = " << m_objects[t].size() << ", p size = " << m_objects[p].size() << endl;
            m_costFunction->loadDataCurentFrames(t, p);
            //cout << "blAtTrackerGraph finish load frames" << endl;

            int curentFrameObjectsNum = m_objects[p].size();
            // X- search neighboors
            // X.1- calculate bucket borders indexes
            //cout << "X.1- calculate bucket borders indexes" << endl;
            int bucketNumber = 10;
            cout << "bucket number = " << bucketNumber << endl;
            vector<int> bucketborders; bucketborders.resize(bucketNumber+1);

            int step = curentFrameObjectsNum / bucketNumber;
            bucketborders[0] = 0;
            //cout << "bucket border " << 0 << "= " << bucketborders[0] << endl;
            for (int c = 1 ; c < bucketNumber ; ++c){
                bucketborders[c] = bucketborders[c-1] + step;
                //cout << "bucket border " << c << "= " << bucketborders[c] << endl;
            }
            bucketborders[bucketNumber] = curentFrameObjectsNum-1;
            //cout << "bucket border " << bucketNumber << "= " << bucketborders[bucketNumber] << endl;

            //cout << "m_L = " << m_L << endl;
            bool stop = false;
            bool add_new;
            while(!stop){
                add_new = false;
                int pass = 0;
                for (int c = 1 ; c < bucketNumber+1 ; ++c){
                    if ( fabs(m_objects[p][bucketborders[c-1]+1][0] - m_objects[p][bucketborders[c]][0]) > 2*m_Lmenus1) {
                        //cout << "objects X = " << m_objects[p][bucketborders[c-1]+1][0] << ", " << m_objects[p][bucketborders[c]][0] << endl;
                        bucketborders.push_back((bucketborders[c-1]+1 + bucketborders[c])/2);
                        pass++;
                        add_new = true;
                    }
                }
                bucketNumber += pass;
                //cout << "bucketNumber = " << bucketNumber << endl;
                std::sort(bucketborders.begin(), bucketborders.end());
                if (add_new == false){
                    stop = true;
                }
            }

            // plot bucket
            //for (int c = 0 ; c < bucketborders.size() ; ++c){
            //    cout << "bucket at" <<  c << " = " << bucketborders[c] << endl;
            //}
            //cout << "bucket number = " << bucketNumber << endl;

            // X.2- Search neighbours objects in next frame
            for (unsigned int nt = 0 ; nt< m_objects[t].size() ; ++nt){
                //cout << "X.2- Search neighbours objects in next frame" << endl;
                bool found_before = false;
                bool found;
                for (int b = 1 ; b <= bucketNumber ; ++b){
                    found = false;
                    int idx = (bucketborders[b-1]+1 + bucketborders[b]) /2;
                    if (  fabs(m_objects[t][nt][0] - m_objects[p][idx][0] ) <= 2*m_Lplus1 ){
                        found_before = true;
                        found = true;

                        // compare all the objects of the bucket
                        int startIdx;
                        if (b-1 == 0){
                            startIdx = bucketborders[b-1];
                        }
                        else{
                            startIdx = bucketborders[b-1]+1;
                        }
                        for (int np = startIdx ; np <= bucketborders[b] ; ++np ){
                            if ( fabs(m_objects[t][nt][0] - m_objects[p][ np ][0] ) <= m_Lplus1 ){
                                if ( fabs(m_objects[t][nt][1] - m_objects[p][ np ][1] ) <= m_Lplus1 ){
                                    blAtConnection *connection = new blAtConnection(m_objects[t][nt], m_objects[p][np], t, p);
                                    //cout << "calculate cost" << endl;
                                    curentCost = m_costFunction->calculateCost(connection);
                                    delete connection;

                                    if (curentCost < costLimit){ // (p-t)*m_L*m_L ?

                                        // add arc to graph
                                        //cout << "add arc to graph" << endl;
                                        int posObject1 = CalculateNodeIdx(t, nt);
                                        int posObject2 = CalculateNodeIdx(p, np);
                                        arc = g.addArc( nodes[posObject1+2], nodes[posObject2+2] );
                                        idArd= g.id(arc);
                                        nodesConnectedArcsIds[posObject1+2].push_back(idArd);
                                        nodesConnectedArcsIds[posObject2+2].push_back(idArd);
                                        cap[arc] = 1;
                                        if (p-t-1 > 0 ){
                                            cost[arc] = int((curentCost/maxCost -1.0 - (p-t-1) +m_jumpEpsilon)*m_coefIntegerConversion);
                                        }
                                        else{
                                            cost[arc] = int((curentCost/maxCost -1.0)*m_coefIntegerConversion);
                                        }
                                        arcs.push_back(arc);
                                        //cout << "add arc to graph done" << endl;
                                    }
                                }
                            }
                        }
                    }
                    else{
                        if (found_before == true && found == false){
                            break;
                        }
                    }
                }
            }
        }
    }
}


void blAtTrackerGraphDetections::buildGraphNegative(lemon::ListDigraph &g, lemon::ListDigraph::ArcMap<int> &cap, lemon::ListDigraph::ArcMap<int> &cost, lemon::ListDigraph::Node &source, lemon::ListDigraph::Node &target, vector<lemon::ListDigraph::Arc> &arcs, vector< vector<int> > &arcsInOutMap){


    // 1- Create the object list
    m_nodesData.clear();
    vector<int> frameBreak;
    int countor = -1;
    for (unsigned int frame = 0 ; frame < m_objects.size() ; ++frame){
        for (unsigned int objIdx = 0 ; objIdx < m_objects[frame].size() ; ++ objIdx){
            countor++;
            m_nodesData.push_back(blAtDetection(m_objects[frame][objIdx], frame, objIdx));
        }
        frameBreak.push_back(countor+1);
    }

    // 2- Create the detection nodes
    vector<lemon::ListDigraph::Node> nodes;
    source = g.addNode();
    target = g.addNode();
    nodes.push_back(source);
    nodes.push_back(target);
    for (unsigned int i = 0 ; i < m_nodesData.size() ; ++i){
        nodes.push_back(g.addNode());
    }

    // 3- create the arcs
    // 3.1- To source and target
    vector<int> arcIOInter; arcIOInter.resize(2);
    lemon::ListDigraph::Arc arc;
    for (unsigned int i = 0 ; i < nodes.size()-2 ; ++i){
        //int frameIdx = m_nodesData.at(i).frameIdx();
        // link to source
        arc = g.addArc(source, nodes[i+2]); cap[arc] = 1; cost[arc] = 0;
        arcs.push_back(arc);
        arcIOInter[0] = 0; arcIOInter[1] = i+2;
        arcsInOutMap.push_back(arcIOInter);
        // link to target
        arc = g.addArc(nodes[i+2], target); cap[arc] = 1; cost[arc] = 0;
        arcs.push_back(arc);
        arcIOInter[0] = i+2; arcIOInter[1] = 1;
        arcsInOutMap.push_back(arcIOInter);
    }

    // 3.2- Between detections
    float maxCost, costLimit;
    if (m_costFunction->isProbability()){
        maxCost = 1.0;
        costLimit = 1.0;
    }
    else{
        maxCost = m_timeSpread*m_L*m_L;
        costLimit = m_L*m_L;
    }
    //cout << "m_frameNumber = " << m_frameNumber << endl;
    //cout << "objects size = " << m_objects.size() << ", " << m_objects[0].size() << endl;
    float curentCost;
    for (unsigned int t=0 ; t<m_frameNumber-1 ; ++t){
        cout << "ConnectionManager -> connect frame: " << t << endl;

        // Get the number of future frame to scan depending on the remaining number of frames
        int endlength = 1;
        if (t < m_frameNumber-1-m_timeSpread ){
            endlength = m_timeSpread;
        }
        else{
            endlength = m_frameNumber-1-t;
        }
        //cout << "endlength = " << endlength << endl;
        // Fill the graph by scanning the connectons
        for (unsigned int p = t+1 ; p <= t+endlength ; ++p){
            cout << "t = " << t << ", p = " << p << endl;
            cout << "t size = " << m_objects[t].size() << ", p size = " << m_objects[p].size() << endl;
            m_costFunction->loadDataCurentFrames(t, p);
            //cout << "blAtTrackerGraph finish load frames" << endl;
            for (unsigned int nt = 0 ; nt< m_objects[t].size() ; ++nt){
                //cout << "\t nt = " << nt << endl;
                for (unsigned int np = 0 ; np<m_objects[p].size(); ++np){
                    //cout << "\t \t np = " << np << endl;
                    blAtConnection *connection = new blAtConnection(m_objects[t][nt], m_objects[p][np], t, p);
                    //cout << "calculate cost" << endl;
                    curentCost = m_costFunction->calculateCost(connection);
                    delete connection;

                    if (curentCost < costLimit){

                        // add arc to graph
                        int posObject1 = CalculateNodeIdx(t, nt);
                        int posObject2 = CalculateNodeIdx(p, np);
                        arc = g.addArc( nodes[posObject1+2], nodes[posObject2+2] );
                        cap[arc] = 1;
                        if (p-t-1 > 0 ){
                            cost[arc] = int((curentCost/maxCost -1 - (p-t-1) +0.01)*m_coefIntegerConversion);
                        }
                        else{
                            cost[arc] = int((curentCost/maxCost -1.0)*m_coefIntegerConversion);
                        }
                        arcs.push_back(arc);

                        // add to maping vector
                        arcIOInter[0] = posObject1+2; arcIOInter[1] = posObject2+2;
                        arcsInOutMap.push_back(arcIOInter);
                    }
                }
            }
        }
    }
}

int blAtTrackerGraphDetections::CalculateNodeIdx( int t, int nt){
    int posCount = 0;
    //if ( t > 0 ){
    for (int i=0; i < t ; ++i){
        posCount += m_objects[i].size();
    }
    //}
    posCount+= nt;
    return posCount;
}

int blAtTrackerGraphDetectionsfindNode(vector<blAtDetection> nodesData, int i, int j){
    //cout << "findNode begin" << endl;
    for (unsigned int count = 0 ; count < nodesData.size() ; ++count){
        if (nodesData.at(count).frameIdx() == i && nodesData.at(count).objectIdx() == j){
            cout << "findNode end" << endl;
            return count;
        }
    }
    //cout << "findNode end" << endl;
    return 0;
}


// for optimization
bool compareX(vector<float> object1, vector<float> object2){
    return (object1[0] < object2[0]);
}

bool compareY(vector<float> object1, vector<float> object2){
    return (object1[1] < object2[1]);
}
