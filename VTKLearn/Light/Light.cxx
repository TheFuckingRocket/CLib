#include <cstdlib>

#include <vtkVersion.h>
#include <vtkLightCollection.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPlaneSource.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkLight.h>
#include <vtkLightActor.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>

int main(int, char *[])
{
  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

  double lightPosition[3] = {1, 0, 0};

  // Create a light
  double lightFocalPoint[3] = {0, 0, 0.5};

  double planeCenter[3] = {0, 0, 0};

  vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
  light->SetLightTypeToSceneLight();

  // The position of the light is defined in the coordinate space indicated by
  // its transformation matrix (if it exists).
  // Thus, to get the light's world space position,
  // use vtkGetTransformedPosition() instead of vtkGetPosition().

  light->SetPosition(lightPosition[0], lightPosition[1], lightPosition[2]);

  light->SetPositional(true);   // required for vtkLightActor below
  light->SetConeAngle(10);      // ConeAngle means the axis of the cone and
                                // a ray along the edge ofthe cone.

  light->SetFocalPoint(lightFocalPoint[0], lightFocalPoint[1], lightFocalPoint[2]);
  light->SetDiffuseColor(1,0,0);    // 散播
  light->SetAmbientColor(0,1,0);    // 周围的
  light->SetSpecularColor(0,0,1);   // 反射的

  vtkLightCollection* originalLights = renderer->GetLights();
  std::cout << "Originally there are " << originalLights->GetNumberOfItems() << " lights." << std::endl;

//  renderer->AddLight(light); // can't do this here - must do this after the renderWindow->Render() below

  // Display where the light is
  vtkSmartPointer<vtkLightActor> lightActor = vtkSmartPointer<vtkLightActor>::New();
  lightActor->SetLight(light);  // add first light.
  renderer->AddViewProp(lightActor); // make lightActor as superclass of all actors, volumes, 2D actors

  // Display where the light is focused
  vtkSmartPointer<vtkSphereSource> lightFocalPointSphere = vtkSmartPointer<vtkSphereSource>::New();
  lightFocalPointSphere->SetCenter(planeCenter);
  lightFocalPointSphere->SetRadius( 0.5 ); // make it bigger. Radius is proportion of map
  lightFocalPointSphere->Update();

  vtkSmartPointer<vtkPolyDataMapper> lightFocalPointMapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
  lightFocalPointMapper->SetInputConnection(lightFocalPointSphere->GetOutputPort());

  vtkSmartPointer<vtkActor> lightFocalPointActor = vtkSmartPointer<vtkActor>::New();
  lightFocalPointActor->SetMapper(lightFocalPointMapper);
  // Actor controls lighting, color, edge, shade, texture etc.
  lightFocalPointActor->GetProperty()->SetColor(1.0, 1.0, 0.0); //(R,G,B)
  // make lightFocalPointActor as superclass of all actors, volumes, 2D actors
  renderer->AddViewProp(lightFocalPointActor);

  // Create a plane for the light to shine on
  vtkSmartPointer<vtkPlaneSource> planeSource = vtkSmartPointer<vtkPlaneSource>::New();
  planeSource->SetResolution(100, 100); // resolution means view size.
  planeSource->Update();

  vtkSmartPointer<vtkPolyDataMapper> planeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION <= 5
  planeMapper->SetInput(planeSource->GetOutput());
#else
  planeMapper->SetInputData(planeSource->GetOutput());
#endif

  vtkSmartPointer<vtkActor> planeActor = vtkSmartPointer<vtkActor>::New();
  planeActor->SetMapper(planeMapper);
  renderer->AddActor(planeActor);

  vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);

  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderWindow->Render();
  std::cout << "Now there are " << originalLights->GetNumberOfItems() << " lights." << std::endl;

  // add second light.
  // renderer->AddLight(light); // must do this after renderWindow->Render();
  // After AddLight(), we can see red color in sphere.
  std::cout << "Now there are " << originalLights->GetNumberOfItems() << " lights." << std::endl;


  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
