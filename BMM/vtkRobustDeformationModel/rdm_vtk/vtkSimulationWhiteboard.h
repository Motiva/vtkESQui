/*=========================================================================

==========================================================================

=========================================================================*/
// .NAME vtkSimulationWhiteboard - a pipeline element that allows modification of an unstrucutred grid
// .SECTION Description
// Use this element to enable simulation in your pipeline. You can add this
// element to all kinds of 'simulators' that simulate things like soft tissue
// collision detection or cutting.
//

#ifndef __vtkSimulationWhiteboard_h
#define __vtkSimulationWhiteboard_h

// VTK includes
#include "vtkUnstructuredGridAlgorithm.h"
#include "vtkUnstructuredGrid.h"
#include "vtkIdList.h"
#include "vtkCollisionEventData.h"
#include "vtkActor.h"

#include <stack>

class vtkTransform;

class VTK_EXPORT vtkSimulationWhiteboard :
    public vtkUnstructuredGridAlgorithm
{
public:
    static vtkSimulationWhiteboard *New();
    vtkTypeMacro(vtkSimulationWhiteboard,vtkUnstructuredGridAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);
    
    
    // Events:
    // =============
    // fired when the pipeline changes 'upstream' (no parameter)
    static const unsigned long UpstreamChangedEvent;
    
    // fired when points where added to the protection list (parameter: point ids)
    static const unsigned long PointsProtectedEvent;
    
    // fired when points where removed from the protection list (parameter: point ids)
    static const unsigned long PointsUnProtectedEvent;
    
    // fired when all point locations have been touched by an operation (no parameter)
    static const unsigned long PositionsModifiedEvent;

    // fired when any collision was detected
    static const unsigned long CollisionDetectedEvent;
    
    // fired after a tetra in the geometry data was subdivided
    // (parameter: )
    static const unsigned long TetraSubdividedEvent;
    
    // fired when the location of a point inside the volume mesh has changed
    // notice that this change should preserve the energy level of the simulation
    static const unsigned long PointLocationInVolumeChangedEvent;
    
    // this event is used to propagate changes (eg. movement) of an actor
    // displaying this whiteboard, parameter is a pointer to the offending
    // actor
    static const unsigned long ActorModifiedEvent;
    
    
    // Description:
    // Invokes an event. If you whant to invoke a event
    // use this method instead of the normal vtk methods.
    void InvokeWhiteboardEvent(unsigned long event, vtkObject *parameter = NULL);
    
    // Description:
    // Returns the event id for a given event name.
    //static unsigned long GetEventId(const char *name);
    
    
    // Description:
    // Sets whether to propagate changes downstream. If an event is evoked
    // that signals changes to the geometry data this event could be signaled
    // to the downstream vtk pipeline (by calling Modified() on the whiteboard.)
    // If propagate changes downstream is enabled the pipeline downstram of the
    // whiteboard will be updated on every rendering after a whiteboard change.
    void SetPropagateChangesDownstream(int enabled) {propagateDownstream = enabled != 0;}
    
    
    // Description:
    // Returns the parameter of the last event that was fired. This method
    // is only valid in event handlers (callbacks) and only returns the
    // parameter of the event that invoked the handler.
    vtkObject* GetEventParameter() {return eventParameters.top();}
    // only for scripting
    const char* GetEventName() {return eventName.top();}
    
    
    void SetCollisionData(vtkCollisionEventData* collisionData);
    
    vtkCollisionEventData* GetCollisionData();
    
    
    
    // Description:
    // Returns the geometric data that is subject to simulation as a
    // vtkUnstructuredGrid. If you change this data invoke the apropriate
    // events on the simulation whiteboard.
    vtkUnstructuredGrid *GetGeometryData();
    
	// Returns the vtkUnstructuredGrid like it was
	// before the last PositionsModifiedEvent
    vtkUnstructuredGrid *GetGeometryDataOld();
    
	void EnableSaveOldData();
	void DisableSaveOldData();
    
    double* GetTetraPositionDelta(int tetraId);
    
    
    
    // Description:
    // Applies the given transformation to all points in the geometric data.
    void ApplyTransformatino(vtkTransform* transform);
    
    
    
	// Needs to be called before the
	// simulator changes any points
	// in order to copy the current data
	void PreSimulation();

	// needs to be called DIRECTLY after the simulator
	// changes the points, before it raises the points modified event.
	// now the old data will be set to what in the PreSimulation() was the
	// current data
	void PostSimulation();




	// Defines the mass of the object
	// that is simulated by this vtkSimulationWhiteboard
	// The default is 1
	void SetMass(double mass);


	// Saves a copy of the geometry data in order to
	// have it available in the next simulation step
	// as the "old positions"
	// Each vtkWhiteboardObserver who is interested in
	// the old data needs to call this method. The current
	// geometry data can then be gotten in the next timestep
	// via GetGeometryDataOld();
	void SetSaveGeometryData(bool save);



    
    // Description:
    // Returns an array of vectors representing the speed of every point on the
    // whiteboard. The size of the array is 3 * {number of points}. The speed
    // of point i is stored as 
    // speed = [x, y, z] = array[i*3, i*3 + 1, i*3 + 2]
	double* GetPointSpeeds() {return this->pointSpeeds;};
    
    
    // Description:
    // Returns an array of doubles representing the mass at each point.
	double* GetPointMasses() {return this->pointMasses;};
    
    
    // Description:
    // Adds some points to the list of protected points. Points allready under
    // protection should not be added again! (This is some sort of a exclusive
    // access flag.)
    // Points with ids stored in this list should only be modified by the 
    // whiteboard observer that requested protection.
    // Note that the protection must be enforced locally inside the observers
    // and that points cannot be protected against upstream changes.
    void AddPointProtection(vtkIdList *list);
    
    // Description:
    // Removes some points form the protection list. Points should be only
    // removed from the whiteboard observer who added them to the list.
    // Points with ids stored in this list should only be modified by the 
    // whiteboard observer that requested protection.
    // Note that the protection must be enforced locally inside the observers
    // and that points cannot be protected against upstream changes.
    void RemovePointProtection(vtkIdList *list);
    
    // Description:
    // Returns the list of points that are currentlu under protection.
    // Points with ids stored in this list should only be modified by the 
    // whiteboard observer that requested protection.
    // Note that the protection must be enforced locally inside the observers
    // and that points cannot be protected against upstream changes.
    vtkIdList* GetProtectedPoints() {return protectedPoints;}
    
    // Description:
    // Call this function if you want to notify observers that an actor
    // associated with this whiteboard has changed.
    void InvokeActorChangedEvent(vtkActor* actor) {
        InvokeWhiteboardEvent(ActorModifiedEvent, actor);
    }
    
protected:
    // Usual algorithm methods:
    virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);
    virtual int FillInputPortInformation(int port, vtkInformation *info);
    
    // The time we last copied the input to the output.
    unsigned long lastInputCopyTime;
    
	// speed for each point
    double* pointSpeeds;

	// mass per mass-point
    double* pointMasses;

	// mass of the whole object
	double mass;

	vtkCollisionEventData* collisionData;


    
//BTX
private:
    
	// the current data
	vtkUnstructuredGrid *geometricData;

	// the data as in the last simulation step
	// a lot of algos need the old position of some
	// objects so we copy only once and do it here
	vtkUnstructuredGrid* geometricDataOld;

	// stores a copy of the currentData before the
	// simulation takes place, this will later
	// become the old data
	vtkUnstructuredGrid* geometricDataTemp;
	
	// determines if the old data is to be provided
	// if you don't need it, disable it to gain
	// some performance
	bool saveOldGeometryData;

    bool updating;

    bool propagateDownstream;

    std::stack<vtkObject*> eventParameters;
    std::stack<const char*> eventName;
    vtkIdList* protectedPoints;
    
    
    vtkSimulationWhiteboard();  // constructor
    ~vtkSimulationWhiteboard(); // destructor
    
    vtkSimulationWhiteboard(const vtkSimulationWhiteboard&) {}; // copy constructor
    void operator=(const vtkSimulationWhiteboard&) {}; // copy assignment
//ETX
};


#endif //#ifndef __vtkSimulationWhiteboard_h





