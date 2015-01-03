// /////////////////////////////////////////////////////////// //
// 			       Mpp ToolBox Readme File
// ////////////////////////////////////////////////////////// //

// Compilation --------------------------------------------//
The code uses openMP, that must be enabled in the compiler
// --------------------------------------------------------//

// Execution ----------------------------------------------//
To run the mpp on a single image use the application 'mpp_graphcut_base' with the inputs:
    1- input File (2D image)
    2- output File (2D image)
    3- parameter File (.txt)
	4- birth mask (2D image)
	
To run the mpp on all the images in a folder use the application 'mpp_graphcut_list' with the inputs:
	1- input folder
	2- input filter (ex *.tif)
	3- output folder
	4- parameter File (.txt)
	5- birth mask 'optionnal'
	
The parameter file contain the list of parameters needed to set up the algorithm with one parameter per line such as:
parameter = value; 	

// ////////////////////// Main Scheme ////////////////////////////
// Objects shapes
use the parameter 'objectShape' to select the object. Possibles values are:
	- blMppObjectManagerCircle
	- blMppObjectManagerCircleIn3d
	- blMppObjectManagerSphere
    - blMppObjectManagerEllipse
    - blMppObjectManagerEllipseIn3d
	- blMppObjectManagerEllipsoid
	- blMppObjectManagerRectangle
	- blMppObjectManagerRectangleIn3d
	- blMppObjectManager2dSilhouette
	- blMppObjectManager2dSilhouetteIn3d
	
// Energy functions
use the parameter 'energyFunction' to select the energy. Possibles values are:
	- blMppEnergyBhattacharyya
    - blMppEnergyGradient
    - blMppEnergyBayesian
    - blMppEnergyPartBhatt
    - blMppEnergyPartDonutBhatt
    - blMppEnergyPartGrad
    - blMppEnergyGradientCompt
    - blMppEnergyGradientComptSym
	- blMppEnergyHeadBhatt
	
// Optimizers
	- blMppOptimizerGraphcut	
	- blMppOptimizerGraphcutRG
	- blMppOptimizerMOGraphcut
	- blMppOptimizerMOGraphcutRG
	- blMppOptimizerTestAll


// ///////////////////// Objects shapes Parameters ///////////////
The construction of an object needs the parameters:
	- blMppObjectManager_borderWidth: is an integer greater than 1 to control the size of the border around the object (default 1)
	- blMppObjectManager_overlapCoef: is a float in [0,1] to control the overlaping between object (0-no overlaping, 1-100% overlaping) (default 0)
	- blMppObjectManager_objectRepresentationType: is a string describing the object representation for the output image. possibles values are:
											- border: plot the border of the object
											- centerpoint: plot the center point of the object
											- centercross: plot a cross in the center of the object
											- full: plot all the pixels inside the object
To create a birth mask, the following parameters can be used
	- blMppObjectManager_birthThresholdInf: do not create object at pixels location where the image intensity is below this parameter. (dafault -1)
	- blMppObjectManager_birthThresholdSup: do not create object at pixels location where the image intensity is above this parameter. (dafault -1)	
									If both MppObject_birthThresholdInf and MppObject_birthThresholdSup equal -1, no birth mask is created
			
// --------- MppCircleBaseObject Parameters 			
blMppObjectManagerCircle_minRadius: minimum radius allowed for an object (in pixels)
blMppObjectManagerCircle_maxRadius: maximum radius allowed for an object (in pixels)
blMppObjectManagerCircle_UseCoordinatesPrecalculation: 1 to create a circle library
												  0 to calculate the circle pixels coordinates for each circle
// --------- MppEllipseBaseObject Parameters
blMppObjectManagerEllipse_minLargeAxe: minimum large axe allowed for an object (in pixels) 
blMppObjectManagerEllipse_maxLargeAxe: maximum large axe allowed for an object (in pixels)
blMppObjectManagerEllipse_minSmallAxe: minimum small axe allowed for an object (in pixels)
blMppObjectManagerEllipse_maxSmallAxe: maximum small axe allowed for an object (in pixels)
blMppObjectManagerEllipse_useGradientDescentForOverlaping:   1 to estimate ellipse overlaping with the gradient descent method
														0 to estimate ellipse overlaping by comparing the pixels inside	

// --------- MppRectangleBaseObject Parameters
blMppObjectManagerRectangle_minWidth: minimum width allowed for an object (in pixels)
blMppObjectManagerRectangle_maxWidth: maximum width allowed for an object (in pixels)
blMppObjectManagerRectangle_minHeight: minimum height allowed for an object (in pixels)
blMppObjectManagerRectangle_maxHeight: maximum height allowed for an object (in pixels)

// --------- MppRectangleIn3DBaseObject Parameters
blMppObjectManagerRectangleIn3d_minWidth: minimum width allowed for an object (in pixels)
blMppObjectManagerRectangleIn3d_maxWidth: maximum width allowed for an object (in pixels)
blMppObjectManagerRectangleIn3d_minHeight: minimum height allowed for an object (in pixels)
blMppObjectManagerRectangleIn3d_maxHeight: maximum height allowed for an object (in pixels)

// --------- MppCircleIn3DBaseObject Parameters 			
blMppObjectManagerCircleIn3d_minRadius: minimum radius allowed for an object (in pixels)
blMppObjectManagerCircleIn3d_maxRadius: maximum radius allowed for an object (in pixels)	 
blMppObjectManagerCircleIn3d_allowedZOverlap: set the number of neighbooring z frames in each direction where objects are not allowed to overlap (default 0)
blMppObjectManagerCircleIn3d_UseCoordinatesPrecalculation: 1 to create a circle library
												  0 to calculate the circle pixels coordinates for each circle

// --------- MppEllipseIn3DBaseObject Parameters
blMppObjectManagerEllipseIn3d_minLargeAxe: minimum large axe allowed for an object (in pixels) 
blMppObjectManagerEllipseIn3d_maxLargeAxe: maximum large axe allowed for an object (in pixels)
blMppObjectManagerEllipseIn3d_minSmallAxe: minimum small axe allowed for an object (in pixels)
blMppObjectManagerEllipseIn3d_maxSmallAxe: maximum small axe allowed for an object (in pixels)
blMppObjectManagerEllipseIn3d_allowedZOverlap: set the number of neighbooring z frames in each direction where objects are not allowed to overlap (default 0)
blMppObjectManagerEllipseIn3d_useGradientDescentForOverlaping: 1 to estimate ellipse overlaping with the gradient descent method
														  0 to estimate ellipse overlaping by comparing the pixels inside	

// --------- MppSphereBaseObject Parameters
blMppObjectManagerSphere_minRadius: minimum radius allowed for an object (in pixels)
blMppObjectManagerSphere_maxRadius: maximum radius allowed for an object (in pixels)

// --------- MppEllipsoidBaseObject Parameters
blMppObjectManagerEllipsoid_mina: minimum large axe allowed for an object (in pixels)
blMppObjectManagerEllipsoid_maxa: maximum large axe allowed for an object (in pixels)
blMppObjectManagerEllipsoid_minb: minimum medium axe allowed for an object (in pixels)
blMppObjectManagerEllipsoid_maxb: maximum medium axe allowed for an object (in pixels)
blMppObjectManagerEllipsoid_minc: minimum small axe allowed for an object (in pixels)
blMppObjectManagerEllipsoid_maxc: maximum small axe allowed for an object (in pixels)
	

// ///////////////////// Energy functions parameter //////////////
All the energy functions needs common parameters that are:
	- blMppEnergy_threshold: Thresold for selecting the object. The range value depend on the energy function type

// ---------- BhattacharyyaEnergy	
For the BhattacharyyaEnergy, additionnal parameters are:
	- blMppEnergyBhattacharyya_gradientDirection: Select the object color: (default +1)
											- (0) both dark and light objects
											- (-1) dark objects
											- (+1) light objects
	- blMppEnergyBhattacharyya_thresholdFunctionD: Parameter for the distance transformation funtion to the range [-1, 1] (default 100) 
	- blMppEnergyBhattacharyya_intensityThSup: Threshold to eliminate object which max intentity is more than this threshold (default -1)
										  Put -1 to ignore this thresholding
	- blMppEnergyBhattacharyya_intensityThInf: Threshold to eliminate object which min intentity is less than this threshold (default -1)
										  Put -1 to ignore this thresholding
	- blMppEnergy_threshold: for BhattacharyyaEnergy the threshold value must be greater than 0	

// ---------- GradientEnergy	
For the GradientEnergy, additionnal parameters are:
	- blMppEnergyGradient_gradientDirection: Select the object color: (default +1)
											- (0) both dark and light objects
											- (-1) dark objects
											- (+1) light objects
	- blMppEnergyGradient_epsilon: nromalisation parameter for the gradient norm (default 0)
	- blMppEnergy_threshold: for GradientEnergy the threshold value must be in ]-1,0]
	
// ---------- BayesianEnergy
	- blMppEnergyBayesian_intensityObject: Select the object color: (default +1)
											- (-1) dark objects
											- (+1) light objects
											
	- blMppEnergyBayesian_classifMapPath: root path of the file to save the initial classification image
	                                      ex: /home/toto then images /home/toto_1.tif and /home/toto_2.tif will be created
										  for object class and background class
	- blMppEnergy_threshold: Not used for this energy
	
	

// ---------- PartBhattEnergy
	- blMppEnergyPartBhatt_D: Parameter for the distance transformation funtion to the range [-1, 1] (default 100);
	- blMppEnergyPartBhatt_varTh: max variance inside the object;
	- blMppEnergy_threshold: for PartBhattEnergy the threshold value must be greater than 0

// ---------- PartGradEnergy
	- blMppEnergy_threshold: for PartGradEnergy the threshold value must be in ]-1,0]
	
// ---------- PartDonutBhattEnergy
	- blMppEnergyPartDonutBhatt_D: Parameter for the distance transformation funtion to the range [-1, 1] (default 100);
	- blMppEnergyPartDonutBhatt_VarTh: max variance inside the object;
	- blMppEnergy_threshold: for PartBhattEnergy the threshold value must be greater than 0
	
// ---------- GradientEnergyCompt
	- blMppEnergyGradientCompt_gradientDirection: Select the object color: (default +1)
											- (0) both dark and light objects
											- (-1) dark objects
											- (+1) light objects
	- blMppEnergyGradientCompt_compt: number of pixel in the object border to be above the threshold
	- blMppEnergy_threshold: for GradientEnergy the threshold value must be in ]-1,0]
// ---------- GradientEnergyComptSym
	- blMppEnergyGradientComptSym_gradientDirection: Select the object color: (default +1)
											- (0) both dark and light objects
											- (-1) dark objects
											- (+1) light objects
	- blMppEnergyGradientComptSym_compt: number of pixel in the object border to be above the threshold
	- blMppEnergyGradientComptSym_directionSym: 'minor' consider the symmetry along the minor axis
										   'major' consider the symmetry along the major axis
	- blMppEnergy_threshold: for GradientEnergy the threshold value must be in ]-1,0]

	
// --------- blMppEnergyHeadBhatt
	- blMppEnergyHeadBhatt_D: Parameter for the distance transformation function to the range [-1, 1] (default 100);
	- blMppEnergyHeadBhatt_orientation: 'major' or 'minor' (default major);
	- blMppEnergy_threshold: for blMppEnergyHeadBhatt the threshold value must be greater than 0

// ///////////////////// Optimizer ////////////////////////////////
// ---------- blMppOptimizerGraphcut
blMppOptimizerGraphcut calculates a Marked Point Process object detection with the multiple
birth and cut strategy. 
The parameters are:

blMppOptimizerGraphcut_ObjectsPerIteration: Set the number of new objects per iterations (default 100)
blMppOptimizerGraphcut_MaxIteration: Set the maximum number of iterations (default 2000)
blMppOptimizerGraphcut_PerturbationProbability: Probability in [0, 1] that allows to propose new objects as a perturbation of already detected objects (default 0)

optionals parameters for verbose are:
blMppOptimizerGraphcut_IntermediateResPath: Set the path where to save intermediate results during the processing (default "" (empty))
								 If this string is empty no intermediate results are saved
blMppOptimizerGraphcut_IterationsVerboseRatio: This integer allows to set the frequency (in iterations) of the printing intermediate results (default 100)
blMppOptimizerGraphcut_integerCoeff: Coefficient that transform the energy value from float to integer (default 1000);
	
// ---------- blMppOptimizerGraphcutRG
blMppOptimizerGraphcutRG calls N times blMppOptimizerGraphcut in parallel and merge the obtained 
objects configurations making a Roland Garros like competition  
The parameters are:
MppGraphCutRolandGarros_threadNumber: Number of blMppOptimizerGraphcut to be ran in parallel
As blMppOptimizerGraphcutRG uses blMppOptimizerGraphcut, you need to set the blMppOptimizerGraphcut parameters.

// ---------- blMppOptimizerMOGraphcut
blMppOptimizerMOGraphcut calculates a Marked Point Process object detection with the multiple
birth and cut strategy with several objects shapes.
The parameters are: the same than MppGraphCut
 

// ---------- blMppOptimizerMOGraphcutRG
As MppGraphCutMultipleObject is computational expensive, blMppOptimizerMOGraphcutRG 
search objects shapes independently using blMppOptimizerGraphcut optimizer. Then a final competition is performed 
to get final object configuration containing all the shapes.
The parameters are: the same than blMppOptimizerGraphcut

// ---------- blMppOptimizerTestAll
blMppOptimizerTestAll calculates an object detection by trying all the possible objects in the image
and keeping the one with an energy higher than the threshold and non conflicted
one to each other.
The parameters are:
blMppOptimizerTestAll_numberThread: Number of thread to make a parallel search in the image (default 1)
	
	
	
	
	
	