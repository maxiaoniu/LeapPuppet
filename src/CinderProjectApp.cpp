#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "Mesh.hpp"
using namespace ci;
using namespace ci::app;
using namespace std;

class CinderProjectApp : public App {
  public:
    Mesh mesh;
    CameraPersp mCam;
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void CinderProjectApp::setup()
{
    
    mesh.LoadMesh("/Users/mayue/astroBoy_walk_Max.dae");
    mCam.lookAt( vec3( 0,0, 20 ), vec3( 0 ) );
    
}

void CinderProjectApp::mouseDown( MouseEvent event )
{
}

void CinderProjectApp::update()
{
}

void CinderProjectApp::draw()
{
    gl::clear( Color::white() );
    gl::color( Color::black() );
    gl::setMatrices( mCam );
    gl::draw(mesh.m_Model);
}

CINDER_APP( CinderProjectApp, RendererGl )
