Display "CYLINDER TEST"  "Screen" "rgbsingle"
Format 800 600

FrameBegin 1

Background 0 0 0

#Iso
CameraEye   5 5 5
CameraAt    0 0 0
CameraUp   0 1 0

CameraFOV  40.0

WorldBegin

Color 1 1 1

Cylinder 2 -1 1 -1

WorldEnd

FrameEnd

