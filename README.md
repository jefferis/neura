Introduction
============
This repository consists of source code derived from [neura.org](http://www.neura.org), specifically the [cfd.zip][cfdurl] source code package.  To this I have added some small fixes to enable compilation with gcc 4 on mac and 64 bit linux. In addition I have added some command line otpions to the anisofilter anisotropic diffusion tool (our main interest in this package). This includes the ability to specify all the main algorithm parameters on the command line and to specify image voxel dimensions for e.g. input tifs.  See [FruCloneClustering repository](https://github.com/jefferis/FruCloneClustering) for how we have been using this code.

    $ ./anisofilter -h
    Usage: anisofilter [Options] inputfilename.tiff  outputfilename.tiff 
    -------------------------------------------
    Options (default values):
    -scanrange:2
    -tau:4.0
    -ipflag:0
    -nsteps:4
    -anicoeff{1,3}:1 1 0
    -lambda: 0.1
    -dx: -dy: -dz: (voxel dimensions)
    -------------------------------------------
    Programmed by Philip J. Broser and Roland Schulte.
    Report bugs to <philip.broser@mpimf-heidelberg.mpg.de>.

[cfdurl]: http://www.neura.org/NeuRA_Homepage/Download_files/cfd.zip

License
=======
The original Neura source code is released under the copyleft GPL license, so my additions retain that license. Copyright of the main code obviously remains with the original authors.