This project is for the winter term 2025/2026 course Seminar Modern Programming Languages at Ruhr-University Bochum.

In this seminar I'm studying the basics of the shading language HLSL, where the focus is on the graphics pipeline, the different shader types and variable types.
I'm making this application to understand the graphics pipeline and the different shader types and to learn the Direct3D11 API.

This project requires microsoft build tools, which are shipped with Visual Studio 2022.

The build script needs to be run from a visual studio developer command prompt, which sets the necessary environment variables.

Running the C version:


Build:
This project can be built by running the c.bat script found in the misc directory. The build output can be found in "/MPL-HLSL/C/build/".

Running:
The working directory has to be the folder that contains the shader source files. The default working directory is "/MPL-HLSL/C/code", as it contains all the shader source files.
From the working directory, execute the build output.


Running the Cpp version:

Build:
This project can be built by running the cpp.bat script found in the misc directory. The build output can be found in "/MPL-HLSL/CPP/build/".

Running:
The working directory has to be the folder that contains the shader source files. The default working directory is "/MPL-HLSL/CPP/code", as it contains all the shader source files.
From the working directory, execute the build output.

I recommend to set the working directory in a solution file and launch the program using visual studio. 
