#include <geometries.h>

using namespace std;
using namespace H3D;

// ---------------------------------------------------------------------------
//  Bounded Physics Model Dialog Declaration
// ---------------------------------------------------------------------------

//class BoundedPhysicsModelDialog: public wxPropertySheetDialog
//{
//DECLARE_CLASS(BoundedPhysicsModelDialog);
//public:
//  BoundedPhysicsModelDialog (wxWindow* parent, ParticleSystem* PS);
//  ~BoundedPhysicsModelDialog ();
//
//  void handleSettingsChange (wxCommandEvent & event);
//  wxPanel* CreateBoundedPhysicsModelSettingsPage(wxWindow* parent);
//
//protected:
//  enum {
//    ID_ENABLED,
//    ID_GEOMETRY,
//  };
//
//  wxImageList* m_imageList;
//  ParticleSystem* ePS;
//  BoundedPhysicsModel* boundedPhysicsModel;
//
//DECLARE_EVENT_TABLE()
//};