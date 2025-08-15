Stroke Graphics...
... is a program I wrote to construct simple stroke-graphic alphabets. 
... that can also be used to create simple stroke-graphic images (if you are so inclined).
... the requirement for which appeared while I was trying to write a test-harness for 
    a LoRa radio.
... the LoRa radio is destined for an embedded application.
... much easier to do the bulk of testing from a Microsoft Windows program.
... as is the original development of the drivers before porting to the embedded 
    platform.
... so, the problem. While I can write programs for Windows I have no inclination to 
    make it my life's work. I wanted some scalable text that would seamlessly change 
    size as the resizeable window was - or could be - resized.
... not so easy to do for dot-matrix based fonts.
... seemed to me characters made of clearly defined 2D coordinates would be more amenable 
    to near-effortless scaling.
... I tried very hard to find an existing tool to construct simple "stick-men" kind of 
    characters.
... bearing in mind I needed a simple file format that could be exported from the 
    construction tool and imported into the test-harness program.
... and then manipulated in the test-harness program using functions I was familiar
    with.
... time was a-wasting. Eventually I realised I was going to have to do it myself.
... in the due course of time "StrokeGraphics" the program (C) evolved.
... not really designed so much as steered manfully towards the goal.
... I knew what the end-result needed to be capable of i.e. one or more alphabets of 
    simple characters built of segments defined simply by their 2D end-point coordinates.
... a simple file format representing each character and it's segments using as little 
    parsing as I could get away with and position-agnostic coordinates : '+' or '-' 
    values around an { x == 0, y == 0 } point on x- and y-axes
... only the character ordinals of the ASCII set { 0 .. 127 }.
... a graphical tool capable of selecting a character for construction.
... a toolset allowing a primitive character to :
    - be created
    - be modified
    - repeatably saved and restored within the tool
    - copied from one character definition to another
    - moved around the construction 'grid'
    - flipped repeatably around both axes relative to the character at any position 
      on the grid
    - exporting the partial or full alphabet into an existing file or a new file
... the 2D grid size is defined on the command-line. Characters are best handled on the 
    grid size they were created in.
... characters are not strictly defined by the grid, having an independent existence 
    based on a simple 'extent' box that contains the entire character.
... as the output file is position-agnostic using relative point values the meaning 
    of 'extent' is also a moveable feast, just as long as characters are fully contained 
    in the box.
... and so : "StrokeGraphics" works well enough to fulfil the goals I set for it. It 
             is no paragon of ergonomics - it is keypress-driven. I would have liked to 
             use a mouse but did not want to afford the time taken to program such a 
             nicety. The layout of the keys I use is not amazing; mostly driven by a 
             decision to use function (Fn) keys where I could as well as any "natural" 
             keys e.g. '<--', '-->' for navigation. By definition it is not well-tested 
             as I made it to enable a single requirement to be consumed by me. On the 
             other hand I have fixed bugs as they arose and gotten rid of annoying 
             "features" - pressing <ESC> exits the program, which I found easy to do 
             by accident releasing voluble oppobrium and epithets. Now it is guarded 
             by a further choice using a Message Box.
             The code is superficially 'C++' but that is only to let me use "gdiplus" 
             as the graphics "language". Mostly it is native 'C' and as structured as I 
             could spare the time to observe; little or no gratuitous data manipulation 
             divorced from functions intended to manage that data. Obeisance to the 
             principles of coupling and cohesion, both between my functions and calls 
             to Windows/operating-system functions. No static data - anywhere.
             I have used a lot of dynamic-memory allocation but only sporadic de-
             allocation; all debts paid when the program closes. Nearly all functions 
             detect and return error events. Mostly they are observed and acted upon 
             by the calling function...

BUGS : (1) all characters are referenced to the character descender y-coordinate. When a 
           program using the alphabet processes a character without a descender reference 
           e.g. '+' such a character can get lowered to the inferred descender y-coordinate
           rather than "floating" at (say) half a character height. Simplest way to avoid 
           this is to add a line-segment at the descender y-coordinate - but put it under 
           the character else the character extent in 'x' will widen.
           STATUS : Fixed and working in "GraphicsAgain001"
           
Developed using : Windows 10 Home 22H2 OS Build 19045.6093  
                  Microsoft Visual Studio Community 2022 (64-bit) Version 17.14.9 Net Version 4.8.09037
                  gVim Version 9
                  Source Insight 4.00.0141
                  Beyond Compare Version 5.0
