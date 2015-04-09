/*******************************************************************************************
*
*  Source File Name = SecurityProjectDemoApp.h
*
*  Descriptive Name = Contains the global definitions, common includes, common required
*                     linker configuration, global classes and also any globally
*                     accessible functions.
*
*  List of Functions/Classes:
*
*     SecurityProjectDemoApp - Class used to render the main application and construct
*                              additional windows based on mouse movement.
*
*     WindowData - Class is used to construct the custom windows every time a mouse moves.
*
*  Dependencies:
*
*     Make sure that all the used header files and default c++ headers are available.
*     This includes OpenGL libraries and cinder libraries.
*
*  Restrictions:
*
*     Make sure that all linker options are set correctly to avoid failures. Compiling supported
*     only on windows 32 bit.
*
*  Supported Platforms:
*       Windows 32 bit
*
*  Project Developers:
*      Devan Shah        (100428864)
*      Miguel Arindaeng  (100394094)
*
*  Change Log:
*    DATE      DESCRIPTION                                         
*  dd/mmm/yyyy
*  ========================================================================================
*  31/03/2015  Initial drop - File contains general class functions used in this app
*  04/03/2015  Adding - Added additional variables to the SecurityProjectDemoApp class
*  04/04/2015  Adding - Added additional variables to the SecurityProjectDemoApp class
*  04/05/2015  Adding - Added additional variables to the SecurityProjectDemoApp class and
*                       new c++ lib for string stream conversion.
*
*******************************************************************************************/

// General C++/windows header files that are needed
#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <sstream>

// Include all the dependent cinder libraries that are needed for
// SecurityProjectDemoApp application.
#include "cinder/app/AppNative.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"

// Include internal resource file for SecurityProjectDemoApp application.
// This resource header is used to load the image that will be displayed
// for the multiple windows that will be opened. This image is needed to
// simulate the crashing of the display driver.
#include "Resources.h"

// Define global namespace used through out the project to use specific variable/functions
// without specifying the full namespace for each call/variable declaration.
using namespace ci ;
using namespace ci::app ;
using namespace std ;
using namespace gl ;

// General default window size and position
#define WINDOW_X_SIZE 960 
#define WINDOW_Y_SIZE 640
#define DEFAULT_WINDOW_X_POS 100
#define DEFAULT_WINDOW_Y_POS 100

/**********************************************************************************

Class Name = SecurityProjectDemoApp

Descriptive Name = This class is used to render the application and allow for
                   creating multiple windows to simulate a display driver crash.

Functions:

    // General Cinder override functions
    prepareSettings ( Settings *settings ) ;
    setup () ;
    mouseDown ( MouseEvent event ) ;
    mouseMove ( MouseEvent event ) ;
    keyDown ( KeyEvent event ) ;
    draw () ;
 
    // Window creation function
    createInfectedWindow ( cinder::DataSourceRef securityImageRef,
                           int windowHeight,
                           int windowXPos,
                           int windowYPos
                         ) ;
Variables:
       // Global Variables across all class functions
       int maxScreenWidth ;
       int maxScreenHeight ;
       bool startInitialized = false;
       int previousMouseX = NULL ;   
       int previousMouseY = NULL ;   
       int mouseDownMouseX = NULL ;  
       int mouseDownMouseY = NULL ;
       int mouseMoveCounter = 0 ;

Dependencies =
    
    None

Restrictions =

    Must have the cinder lib's available in the linker and c/c++ configurations. 

******************************************************************************/
class SecurityProjectDemoApp : public AppNative
{
    // Defined the public functions and variables that are used through out the class
    public:
       // Default setup functions used in this class.
       // Theses functions are predefined to construct a cinder window using openGL.
       // Description of each function can be found where there function is defined
       // SecurityProjectDemoApp.cpp.
       void prepareSettings ( Settings *settings ) ;
       void setup () ;
       void mouseDown ( MouseEvent event ) ;
       void mouseMove ( MouseEvent event ) ;
       void keyDown ( KeyEvent event );
       void draw () ;

       // Custom window creation functions
       void createInfectedWindow ( cinder::DataSourceRef securityImageRef,
                                   int windowWidth, 
                                   int windowHeight,
                                   int windowXPos,
                                   int windowYPos
                                 ) ;
       // Global Variables across all class functions
       bool startInitialized = true ; // Boolean value to show if virus has been triggered
       int previousMouseX    = NULL ; // Keeps track of the previous mouse x position
       int previousMouseY    = NULL ; // Keeps track of the previous mouse y position
       int mouseDownMouseX   = NULL ; // Keeps track of the mouse x position when mouse button is pressed
       int mouseDownMouseY   = NULL ; // Keeps track of the mouse y position when mouse button is pressed
       int mouseMoveCounter  = 0 ;    // Stores how many time the moveMouse function is called.
       int maxScreenWidth ;           // Stores the maximum screen width
       int maxScreenHeight ;          // Stores the maximum screen height
} ;

/**********************************************************************************

Class Name = WindowData

Descriptive Name = This class is used to define the window to be created and stores
                   the color and the texture (image) that the window is to display.

Function =

    Only has a public constructor and some object declarations (Color and Texture).

Restrictions =

    Must have the cinder lib's available in the linker and c/c++ configurations.

******************************************************************************/
class WindowData
{
    // Defined the public functions and variables that are used through out the class
    public:
       // Constructor of the class that is used to store the color of the window and the image to display.
       // Use initialization lists to initialize the window background color to white
       WindowData ( Texture image ) : windowBackgroundColor ( Color ( 255, 255, 255 ) )
       {
          // Store the image in the global variable to access when needed
          windowImageToLoad = image ;
       }

       // Variables to store the object in the class
       Color   windowBackgroundColor ; // Default will be white set above
       Texture windowImageToLoad ;     // Stores the Texture image object
} ;