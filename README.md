# ImageDSplayground
Codebase for generating visuals by modifying pixels with dynamical systems over cyclic rings, as described in my senior thesis at Harvey Mudd (to be released in May 2023). 
### Installation
To run the program, you will need to download OpenFrameworks by following the instructions [here](https://openframeworks.cc/download/).  
Once you have OF downloaded, navigate to the folder projectGenerator and open the Project Generator application. Give your project a title and click Generate.  
<img src = "/bin/data/nav1.png" width = "50%" height = "50%">  

<img src = "/bin/data/nav2.png" width = "50%" height = "50%">
Next, navigate to the folder apps, then click on myApps and find your project. Replace the contents of the folders src and bin/data with those of this repo.  
bin/data contains a bunch of images, but feel free to add your own images. Images at the beginning of the folder will be retrieved first by the program.  

### Running the Program
To open the project, click yourprojectname.sln (Windows) or yourprojectname.xcodeproj (Mac). This should open up the project in your IDE. You can then run the project to test if it's working. Press any key on your keyboard to transition from one image to another.
### Modifying parameters, update functions, and transition functions.
Navigate to ofApp.cpp in the src folder. You can modify the transition function inside setup():  
<img src = "/bin/data/nav4.png" width = "50%" height = "50%">  
You can modify the update function in UDS() by commenting and uncommenting update functions, and/or changing parameters:
<img src = "/bin/data/nav5.png" width = "50%" height = "50%">  
You can also modify and add your own custom transition and update functions. Declare the function in ofApp.h and write it in ofApp.cpp. You can look at the update and transition functions in ofApp.cpp for examples.
