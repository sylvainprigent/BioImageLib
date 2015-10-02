// /////////////////////////////////////////////////////////// //
// 			AssociationTracking ToolBox Readme File
// ////////////////////////////////////////////////////////// //

// Compilation --------------------------------------------//
The code uses LEMON graph library 1.2.3
// --------------------------------------------------------//

/ Execution ----------------------------------------------//
To run the AssociationTracking from data stored in a single folder, use 'AssociationTracking' with the inputs:
        1- input images folder (2D images)
        2- image file extension (ex .tif)
        3- detections files folder (.txt files)
        4- parameter File (.txt)
        5- output image name
        6- begin image index (optional)
        7- end image index (optional)
		
The parameter file contain the list of parameters needed to set up the algorithm with one parameter per line such as:
parameter = value; 	

// ////////////////////// Main Scheme ////////////////////////////
// Costs functions
	- AT_CostEuclidian
	- AT_CostAxonVesselness
	
// Optimizers
	- AT_TrackerGraphDetections
	
	
// ///////////////////// Costs functions ///////////////
// AT_CostEuclidian
AT_CostEuclidian doesn't have any parameters

// AT_CostAxonVesselness
	- AT_CostAxonVesselness_vesselnessImagesFolder : Address of the folder containing pre-calculated vesselness images for each frames
    - AT_CostAxonVesselness_maxMovX : Maximum distance (in X) between two detection for a connection to be considered
    - AT_CostAxonVesselness_maxMovY : Maximum distance (in Y) between two detection for a connection to be considered
    - AT_CostAxonVesselness_maxMovZ : Maximum distance (in Z) between two detection for a connection to be considered
    - AT_CostAxonVesselness_threshold : Threshold to select the maximum vesselness value for a connection to be considered 
	                                    (0 remove all connection, 1 keep all possible connection)


// ///////////////////// Optimizers ///////////////
All the optimizers have the parameter:
	- AT_Tracker_timeSpread : Distance between frames where a connection is allowed. 
							  1- Only objects from consecutive frames are connected
							  2- A connection between objects can jump over one frame
							  3- A connection between objects can jump over two frame
							  ...
							  
// AT_TrackerGraphDetections: This optimizer build a graph where each node is a detection. Arcs are added between nodes with a negative cost calculated
                              from the given cost function. 
	- AT_TrackerGraph_L : Maximum distance (in X and Y) between two detection for a connection to be considered. This parameter is used for speed in the 
	                      connections search
	- AT_TrackerGraph_optimization : Method used to optimize the graph. Available methods are 'shortestPath' and 'minCostFlow'	(default 'shortestPath') 
	- AT_TrackerGraph_coefIntegerConversion : Multiplication coefficient to convert the cost from float into int (default 100). It allows to save memory for graph storing
	- AT_TrackerGraph_jumpEpsilon : Epsilon coefficient to penalize a connection that jump over frames (0= no penalization, default 0.01) 
	- AT_TrackerGraph_requiredFlow : Number of tracks to get (parameter used only for 'minCostFlow' optimization) (default 100)
