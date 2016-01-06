#include "blPfParticle.h"
#include <iostream>
using namespace std;

blPfParticle::blPfParticle(blPfState* initialState){
    m_states.push_back(initialState);
    m_stateIdxs.resize(1);
    m_stateIdxs[0] = initialState->getStateAt(0);
    m_initialFrameIdx = initialState->getStateAt(0);
    m_n = -1;
}

blPfParticle::blPfParticle(vector<blPfState*> &particleData){
    m_states = particleData;
    m_stateIdxs.resize(m_states.size());
    for (unsigned int i = 0 ; i < m_states.size() ; ++i){
        m_stateIdxs[i] = i;
    }
    m_n = -1;
}

blPfParticle::~blPfParticle(){

    //cout << "blPfParticle::~blPfParticle() size to delete = " << m_states.size() << endl;
    for (unsigned int i = 0 ; i < m_states.size() ; ++i){
        delete m_states[i];
    }
}

void blPfParticle::addState(blPfState* state){


    if (m_n > -1){
        if (int(m_stateIdxs.size()) >= m_n){
            // move idxs by -1
            //cout << "move idxs by -1" << endl;
            for (unsigned int i = 0 ; i < m_stateIdxs.size() -1 ; ++i){
                m_stateIdxs[i] = m_stateIdxs[i+1];
            }
            m_stateIdxs[m_stateIdxs.size() -1] = state->getStateAt(0);

            // move states by -1
            //cout << "move states by -1, " ;
            //delete m_states[0];
            //blPfState* inter = m_states[0];
            for (unsigned int i = 0 ; i < m_states.size() -1 ; ++i){
                m_states[i] = m_states[i+1];
            }
            m_states[m_states.size() -1] = state;
            //delete inter;

        }
        else{
            m_states.push_back(state);
            m_stateIdxs.push_back(state->getStateAt(0));
        }
    }
    else{

        m_states.push_back(state);
        m_stateIdxs.push_back(state->getStateAt(0));
    }

    /*
    if (m_n > -1){
        if (m_stateIdxs.size() > m_n){
            // delete the first one
            m_stateIdxs.erase(m_stateIdxs.begin());
            blPfState* state =  m_states[0];
            m_states.erase(m_states.begin());

            //cout << "removed state = " << state->getState().at(0) << ", "<< state->getState().at(1) << ", "<< state->getState().at(2) << endl;
            delete state;
        }
    }
    */

/*
    cout << "add state: m_stateIdxs: ";
    for (int i = 0 ; i < m_stateIdxs.size() ; ++i){
        cout << m_stateIdxs[i] << ", ";
    }
    cout << "--  add state: m_states: ";
    for (int i = 0 ; i < m_states.size() ; ++i){
        cout << m_states[i]->getStateAt(0) << ", ";
    }
    cout << "add state end "<<endl;
    */


}

void blPfParticle::setStateAt(int i, blPfState* state){

    /*
    cout << "set state at: m_stateIdxs: " << i << ": ";
    for (int t = 0 ; t < m_stateIdxs.size() ; ++t){
        cout << m_stateIdxs[t] << ", ";
    }
    cout << endl << "set state at state: m_states: " << i << ": ";
    for (int t = 0 ; t < m_states.size() ; ++t){
        cout << m_states[t]->getStateAt(0) << ", ";
    }
    cout << endl;
    */

    if (m_n > -1){
        int pos = findPos(i);
        m_states[pos] = state;
    }
    else{
        m_states[i-m_initialFrameIdx] = state;
    }


}

blPfState* blPfParticle::getStateAt(int i){
    //cout << "blPfParticle::getStateAt begin" << endl;
    //cout << "pos = " << findPos(i) << ", asked: " << i << endl;

/*
    cout << "get state at: m_stateIdxs: " << i << ": ";
    for (int t = 0 ; t < m_stateIdxs.size() ; ++t){
        cout << m_stateIdxs[t] << ", ";
    }
    cout << endl;
    */

    if (m_n > -1){
        int pos = findPos(i);
        //cout << "founded state frame = "<<m_states[pos]->getStateAt(0) << endl;
        return m_states[pos];
    }
    else{
        return m_states[i-m_initialFrameIdx];
    }
}

void blPfParticle::setStateAt(int i, int posState, float valstate){
    //cout << "blPfParticle::setStateAt begin" << endl;

    /*
    cout << "set state pos  at: m_stateIdxs: " << i << ": ";
    for (int t = 0 ; t < m_stateIdxs.size() ; ++t){
        cout << m_stateIdxs[t] << ", ";
    }
    cout << endl << "set state pos at state: m_states: " << i << ": ";
    for (int t = 0 ; t < m_states.size() ; ++t){
        cout << m_states[t]->getStateAt(0) << ", ";
    }
    cout << endl;
    */

    if (m_n > -1){
        m_states[findPos(i)]->setStateAt(posState, valstate);
    }
    else{
        m_states[i-m_initialFrameIdx]->setStateAt(posState, valstate);
    }

    //cout << "blPfParticle::setStateAt end" << endl;
}

void blPfParticle::keepOnlyLastnStates(int n){
    //cout << "keepOnlyLastnStates begin" << endl;
    m_n = n;
    if (int(m_stateIdxs.size()) > m_n){
        cout << "remove one state, size = " << m_stateIdxs.size() << endl;
        while (int(m_stateIdxs.size()) > m_n){
            m_stateIdxs.erase(m_stateIdxs.begin());
            delete m_states[0];
            m_states.erase(m_states.begin());
        }
    }
    //cout << "keepOnlyLastnStates end" << endl;
}

// protected
int blPfParticle::lastIdx(){
    return m_stateIdxs[m_stateIdxs.size()-1];
}

int blPfParticle::findPos(int i){
    //cout << "blPfParticle::findPos ";
    int pos = -1;
    for (unsigned int t = 0 ; t < m_stateIdxs.size() ; ++t){
        //cout << m_stateIdxs[t] << ", " << endl;
        if (m_stateIdxs[t] == i){
            pos = t;
            return pos;
        }
    }
    //cout << "blPfParticle::findPos, " << i << "not found" << endl;
    return pos;

}

void blPfParticle::deleteStateAt(int i){
    delete m_states[i-m_initialFrameIdx];
    m_states[i-m_initialFrameIdx] = NULL;
}

int blPfParticle::size(){
    return m_states.size();
}

void blPfParticle::removeInitialState(){

    m_stateIdxs.erase(m_stateIdxs.begin());
    blPfState* state =  m_states[0];
    m_states.erase(m_states.begin());
    //cout << "removed state = " << state->getState()[0] << ", "<< state->getState()[1] << ", "<< state->getState()[2] << endl;

    delete state;

}
