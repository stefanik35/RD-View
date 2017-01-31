Display "CUBE TEST"  "Screen" "rgbsingle"
Format 800 600

FrameBegin 1

Background 0 0 0

#Iso
CameraEye   5 5 5
CameraAt    0 0 0
CameraUp   0 1 0

CameraFOV  40.0

WorldBegin

Ka 0.2
Kd 0.8

AmbientLight 1.0 1.0 1.0 1.0

PointLight 5 -5 5 1.0 0.0 1.0 1.0

PointLight 0 -5 0 1.0 0.0 0.0 1.0
PointLight 5 0 0 0.0 1.0 0.0 1.0
PointLight 0 0 5 0.0 0.0 1.0 1.0

#FarLight -5 5 -5 0.0 1.0 0.0 1.0

Color 1.0 1.0 1.0

Cube

WorldEnd

FrameEnd

