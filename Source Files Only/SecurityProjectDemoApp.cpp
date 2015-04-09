/*******************************************************************************************
*
*  Source File Name = SecurityProjectDemoApp.cpp
*
*  Descriptive Name = Main c++ file used to render the SecurityProjectDemoApp application.
*                     This application will act as a simulation of a macro/Trojan that is
*                     infecting a computer. This is for demo only and should not be 
*                     distributed. This application should cause the display driver to 
*                     crash because it will use up all the memory.
*
*  List of Functions:
*
*     SecurityProjectDemoApp::prepareSettings ( Settings *settings )
*     SecurityProjectDemoApp::setup ()
*     SecurityProjectDemoApp::mouseDown ( MouseEvent event )
*     SecurityProjectDemoApp::mouseMove ( MouseEvent event )
*     SecurityProjectDemoApp::keyDown ( KeyEvent event )
*     SecurityProjectDemoApp::draw ()
*     SecurityProjectDemoApp::createInfectedWindow ( cinder::DataSourceRef securityImageRef, 
*                                                    int windowWidth, 
*                                                    int windowHeight,
*                                                    int windowXPos,
*                                                    int windowYPos
*                                                  )
*
*  Dependencies: See function dependencies
*
*  Restrictions: See function dependencies
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
*  31/03/2015  Initial drop - Main file for the application used to render the main app and
*                             create additional windows when mouse is moved.
*  01/04/2015  Implemented - Creating infected window when mouse left or right click is pressed
*  02/04/2015  Implemented - Creating infected window when keyboard key is pressed
*  03/04/2015  Implemented - Creating infected window when mouse is moved
*  04/04/2015  Fixes - fixed issues with windows opening too rapidly when there is no movement
*  04/04/2015  Added - Additional debug code to see when functions are called relative to the
*                      action that the user performs.
*  05/04/2015  Added - A mouseMove counter to allow only opening one window for every 5 mouse
*                      movements.
*
*******************************************************************************************/

// Include the main header file for SecurityProjectDemoApp application project
#include "SecurityProjectDemoApp.h"

/**********************************************************************************

Function Name = SecurityProjectDemoApp::prepareSettings

Descriptive Name = Used to setup the settings for the main application window

Function =

    This function is used to prepare the main settings for the window

Input =
    
    Settings *settings - The settings variable that is created on class creation.

Output =
   
   Currently there are no outputs from this functions as it only performs the action.

******************************************************************************/
void SecurityProjectDemoApp::prepareSettings ( Settings *settings )
{
    settings->setWindowSize ( 0, 0 ) ;
    settings->setFrameRate ( 60.0f ) ;
    settings->setBorderless ( true ) ;
    settings->setWindowPos ( 0, 0 ) ;
}

/**********************************************************************************

Function Name = SecurityProjectDemoApp::setup

Descriptive Name = Setup function for environment settings and also for creating the
                   main screen objects.

Function =

    This function is responsible for performing the setup that is used to get the 
    screen size currently being used. Furthermore also construct one single virus
    infection window.

Output =
   
   Creates the infected window. 
   Currently there are no outputs from this functions as it only performs the action. 

******************************************************************************/
void SecurityProjectDemoApp::setup ()
{
    // Get maximum width and height of the screens
    maxScreenWidth  = GetSystemMetrics ( SM_CXSCREEN ) ;
    maxScreenHeight = GetSystemMetrics ( SM_CYSCREEN ) ;

    // For the default window we need to provide an instance of WindowData
    createInfectedWindow ( loadResource ( RES_SUPPORTEDGESTURES_IMAGE ),
                           WINDOW_X_SIZE,
                           WINDOW_Y_SIZE,
                           DEFAULT_WINDOW_X_POS,
                           DEFAULT_WINDOW_Y_POS
                         ) ;
}

/**********************************************************************************

Function Name = SecurityProjectDemoApp::mouseDown

Descriptive Name = Used to detect the mouseDown action and open the infected window.

Function =

    This function is used to detect the mouseDown by the user and open up an infected
    window at the position where the mouse down was performed. The function also
    performs additional checks to make sure that only a mousedown event is handled. 

Input =
    
    MouseEvent event - The mouse event that is detected for cinder to call the function.

Output =
   
   Creates the infected window and opens it. 
   Prints debug info also.

******************************************************************************/
void SecurityProjectDemoApp::mouseDown ( MouseEvent event )
{
    // Variable Declaration 
    string messageBuffer = "" ;
    ostringstream intToStringConverter ;
    wstring stringTemp ;
    LPCWSTR debugMessage ;

    // Set the mouse positions when mouse button is pressed
    mouseDownMouseX = event.getX () ;
    mouseDownMouseY = event.getY () ;

    // Make sure to only open up the infection window if infection sequence is initialized, 
    // the current event is not already handled and the event is a left or right button click.
    if ( startInitialized && ( event.isLeft() || event.isRight() ) && event.isHandled () == false )
    {
        // ------------------------ Start of Debugging Block -----------------------------

        OutputDebugStringW ( L"\n-------------------- START Detecting a Left/Right mouseDown --------------------\n" ) ;

        // Add all information that is needed into the string stream
        intToStringConverter << "Y--> " << mouseDownMouseY << "\n" << "X--> " << mouseDownMouseX << "\n" ;

        // Fetch the string equivalence of the input (int or any other data type)
        messageBuffer = intToStringConverter.str () ;

        // Convert the messageBuffer string into wstring for later conversion
        stringTemp = wstring ( messageBuffer.begin (), messageBuffer.end () ) ;

        // Convert wstring to LPCWSTR
        debugMessage = stringTemp.c_str () ;

        OutputDebugStringW ( debugMessage ) ;

        OutputDebugStringW ( L"-------------------- DONE Detecting a Left/Right mouseDown --------------------\n" ) ;

        // ------------------------ End of Debugging Block -----------------------------

        // Create the infection window for draw function to open later
        createInfectedWindow ( loadResource ( RES_SUPPORTEDGESTURES_IMAGE ),
                               WINDOW_X_SIZE,
                               WINDOW_Y_SIZE,
                               event.getX (),
                               event.getY ()
                             ) ;

        // Once the single event has been handled mark it as handled to make sure that
        // we avoid handling the same event multiple times.
        event.setHandled( true ) ;
    }
}

/**********************************************************************************

Function Name = SecurityProjectDemoApp::mouseMove

Descriptive Name = Used to detect the mouseMove action and open the infected window.

Function =

    This function is used to detect the mouse move by the user and open up an infected
    window at the position where the mouse moved to. The function also
    performs additional checks to make sure that only a mousemove event is handled. 
    There are some glitches in the cinder mouse down and mouse move detection therefore
    require additional checks to make sure that it was a mouse move and not anything else.

Input =
    
    MouseEvent event - The mouse event that is detected for cinder to call the function.

Output =
   
   Creates the infected window and opens it. 
   Prints debug info also.

******************************************************************************/
void SecurityProjectDemoApp::mouseMove ( MouseEvent event )
{
    // Variable Declaration
    int mouseXDifference ;
    int mouseYDifference ;
    string messageBuffer = "" ;
    ostringstream intToStringConverter ;
    wstring stringTemp ;
    LPCWSTR debugMessage ;

    // In the case that the program has just started set the current mouse position as previous
    if ( previousMouseX == NULL && previousMouseY == NULL && startInitialized )
    {
        previousMouseX = event.getX () ;
        previousMouseY = event.getY () ;
    }
    else
    {
        // Calculate the difference between previous mouse location and current mouse location.
        mouseXDifference = abs ( previousMouseX - event.getX () ) ;
        mouseYDifference = abs ( previousMouseY - event.getY () ) ;

        // Only create the infected windows if it is initialized, the event has not already been handled, there is at least a
        // pixel difference of 5 between mouse y coordinates or mouse x coordinates, the event is not left or right click and the
        // mouseDownX and mouseDownY positions are not the same as the mouse move events.
        if ( startInitialized && event.isHandled () == false && ( mouseYDifference > 10 || mouseXDifference > 10 ) && 
             event.isLeft () == false && event.isRight () == false && mouseDownMouseX != event.getX () && mouseDownMouseY != event.getY ()
           )
        {
            // Increase the mouse move counter to show that this function was called to open up a infection window.
            // This counter will be used to allow not to open too many windows too fast or else can not actually
            // see the windows images as they open. Also the program will crash too fast as there will be a lot of
            // mouse movements. 
            mouseMoveCounter++ ;

            // ------------------------ Start of Debugging Block -----------------------------

            OutputDebugStringW ( L"\n-------------------- START Detecting the mouse moving --------------------\n" ) ;

            // Add all information that is needed into the string stream
            intToStringConverter << "Y--> " << mouseYDifference << "\n" << "X--> " << mouseXDifference << "\n" << "Mouse Move Counter--> " << mouseMoveCounter << "\n" ;

            // Fetch the string equivalence of the input (int or any other data type)
            messageBuffer = intToStringConverter.str () ;

            // Convert the messageBuffer string into wstring for later conversion
            stringTemp = wstring ( messageBuffer.begin (), messageBuffer.end () ) ;

            // Convert wstring to LPCWSTR
            debugMessage = stringTemp.c_str () ;

            OutputDebugStringW ( debugMessage ) ;
            
            OutputDebugStringW ( L"-------------------- END Detecting the mouse moving --------------------\n" );

            // ------------------------ End of Debugging Block -----------------------------

            // Only open the infection window if more then 5 mouseMoves are detected.
            if ( mouseMoveCounter > 5 )
            {
                // Create the infection window for draw function to open later
                createInfectedWindow ( loadResource ( RES_SUPPORTEDGESTURES_IMAGE ),
                                       WINDOW_X_SIZE,
                                       WINDOW_Y_SIZE,
                                       event.getX (),
                                       event.getY ()
                                     ) ;

                // Reset the mouseMove counter to 0 once a single window is opened for every 5 mouse moves.
                mouseMoveCounter = 0 ;
            }

            // Once the single event has been handled mark it as handled to make sure that
            // we avoid handling the same event multiple times.
            event.setHandled ( true );

            // Set the current mouse x and y coordinates as previous for later comparisons
            previousMouseX = event.getX ();
            previousMouseY = event.getY ();
        }
    }
}

/**********************************************************************************

Function Name = SecurityProjectDemoApp::keyDown

Descriptive Name = Used to detect the keyDown action and open the infected window.

Function =

    This function is used to detect the keyDown by the user and open up an infected
    window at a random position on the screen. THere are 3 keystrokes that are reserved
    to perform specific actions in the program, for example:
        SHIFT + c ---> Kill the program
        SHIFT + s ---> Start the infection sequence
        SHIFT + p ---> Pause the infection sequence
    Note: These are needed because this is part of a school project and should not be
          used to actually kill the computers.

Input =
    
    KeyEvent event - The keyboard event that is detected for cinder to call the function.

Output =
   
   Creates the infected window and opens it based on the key that is entered.
    SHIFT + c ---> Kill the program
    SHIFT + s ---> Start the infection sequence
    SHIFT + p ---> Pause the infection sequence
   Prints debug info also.

******************************************************************************/
void SecurityProjectDemoApp::keyDown ( KeyEvent event )
{
    // Get random X and Y coordinates where window should be places.
    // In the range of 10 to max screen size
    int randImageXPoint = rand () % maxScreenWidth + 10 ; 
    int randImageYPoint = rand () % maxScreenHeight + 10 ;

    // Only terminate the program if SHIFT + c is entered with keyboard otherwise
    // Keep on creating windows with the image over and over again at a fixed location.
    // Also only create windows if first SHIFT + s was entered. 
    if ( event.getChar () == 'C' )
    {
       OutputDebugStringW ( L"-------------------- Exit Infection Sequence --------------------\n" );

       exit ( EXIT_FAILURE ) ;
    }
    // if SHIFT + s is detected start the infection sequence
    else if ( event.getChar () == 'S' && startInitialized == false )
    {
        startInitialized = true;

        OutputDebugStringW ( L"-------------------- Starting Infection Sequence --------------------\n" );
    }
    // if SHIFT + p is detected pause the infection sequence
    else if ( event.getChar () == 'P' && startInitialized == true )
    {
        startInitialized = false;

        OutputDebugStringW ( L"-------------------- Pause Infection Sequence --------------------\n" );
    }
    // If there was any other character detected open up the infection window
    else if ( event.getChar() != NULL )
    {
        // Make sure to only open up the infection window if infection sequence is initialized, 
        // the current event is not already handled. 
        if ( startInitialized && event.isHandled () == false )
        {
            OutputDebugStringW ( L"-------------------- Detected a keyDown --------------------\n" );

            // Create the infection window for draw function to open later
            createInfectedWindow ( loadResource ( RES_SUPPORTEDGESTURES_IMAGE ),
                                   WINDOW_X_SIZE,
                                   WINDOW_Y_SIZE,
                                   randImageXPoint,
                                   randImageYPoint
                                  ) ;

            // Once the single event has been handled mark it as handled to make sure that
            // we avoid handling the same event multiple times.
            event.setHandled ( true );
        }
    }
}

/**********************************************************************************

Function Name = SecurityProjectDemoApp::draw

Descriptive Name = Draw the windows that have been created, by default it will create the
                   the main application window

Function =

    This function is responsible for drawing the major windows with all the data that is
    present in the windowData objects.

Output =
   
   Currently there are no outputs from this functions as it only performs the action. 

******************************************************************************/
void SecurityProjectDemoApp::draw ()
{
    // Set the color to white for the background
    gl::clear ( Color ( 255, 255, 255 ) );

    // Get the data for the window 
    WindowData *data = getWindow ()->getUserData<WindowData> ();

    // Only try to extract and draw the window if there is window data available
    if ( data != NULL )
    {
        // Set the color based of the data in the window object
        gl::color ( data->windowBackgroundColor );

        // Load the image that is available in the window data object
        gl::draw ( data->windowImageToLoad, Vec2f ( 0, 0 ) );
    }

    // Enable alpha bending, and set some color conversion settings
    gl::enableAlphaBlending ();
    glColor4f ( 1.0f, 1.0f, 1.0f, 0.5f );
    glEnable ( GL_BLEND );
    gl::end ();
}

/**********************************************************************************

Function Name = SecurityProjectDemoApp::createInfectedWindow

Descriptive Name = Create custom image with data inside.

Function =

    This function is used to create windows based on the configuration that is passed in
    to the function. Also supports adding images into the windows while creating the image.

Input =
    
    std::string securityImageRef  - The location of the image that needs to be displayed
    int windowWidth               - The width of the window
    int windowHeight              - The height of the window
    int windowXPos                - The x position where the window is to be placed
    int windowYPos                - The y position where the window is to be placed

Output =
   
   Currently there are no outputs from this functions as it only performs the action. 

******************************************************************************/
void SecurityProjectDemoApp::createInfectedWindow ( cinder::DataSourceRef securityImageRef, int windowWidth, int windowHeight, int windowXPos, int windowYPos )
{
    // Load the image that was passed in into a proper format
    gl::Texture securityImage = gl::Texture ( loadImage ( securityImageRef ) );
    
    // Create the window of size passed in, set the image into the windowData object and set it as borderless and
    app::WindowRef newWindow = createWindow ( Window::Format ().size ( windowWidth, windowHeight ) ) ;
    newWindow->setUserData ( new WindowData ( securityImage ) );
    newWindow->setBorderless ( true ) ;
    newWindow->setPos ( windowXPos, windowYPos ) ;
    newWindow->setTitle ( "SecurityProjectDemoApp" ) ;

    // Get a unique id for the window that is used to identify the window
    int uniqueId = getNumWindows () ;

    // Set the unique number for the new window that was created
    newWindow->getSignalClose ().connect (
        [uniqueId, this]
        {
           this->console () << "You closed window #" << uniqueId << std::endl ; // Action performed when the window is closed
        }
    ) ;
}

CINDER_APP_NATIVE ( SecurityProjectDemoApp, RendererGl )
