# MeleeCameraUE5

![Showcase](/showcase.gif "Showcase")

# Description
This project aims to recreate the look and feel of Melee's camera through the use of decompiled source code from the game. The original purpose of this project was only to decompile Melee's camera code, but in the process it became a learning experience for development in Unreal Engine.

![Bounds](/bounds.gif "Bounds")

# Stage

### Fixedness
Fixedness is a scale factor that controls how much the camera will rotate in response to player movement. In general, the higher the amount of subjects (generally players) in the scene the higher the fixedness value will be set.
```
float fixedness;
```
### Camera Limits
The maximum dimensions of the camera bounding box are set for each side of the screen. These are accompanied by depth limits that control how far the camera is allowed to 'zoom' in and out to.
#### Camera bound limits
```
float cam_limit_top;
float cam_limit_right;
float cam_limit_bottom;
float cam_limit_left;
```
#### Camera depth limits
```
float cam_dist_max;
float cam_dist_min;
```

### Rotation
These values control how much the camera will rotate horizontally / vertically in response to the subjects in the scene.
```
float vertical_rot;
float horizontal_rot;
```
### Tilt
The base vertical tilt of the camera in degrees, default is -10.0
```
float tilt;
```
### Camera Offset
The offset at which the camera will begin to rotate horizontally / vertically
```
FVector2D cam_offset;
```

# CamSubject

### Direction
Current direction the subject is facing, `-1.0` or `1.0`
```
float dir;
```
### Default Bounds
The default bounding box of the subject, used to calculate the camera bounds.
```
FBox2D default_bounds;
```
### Focus
The point of focus for the camera.
```
FVector focus;
```
### Position
Current position of the subject.
```
FVector pos;
```
### Y Offset
Vertical offset of the bounding box from the focus point.
```
float y_offset;
```

# MainCamera
### Interest
```
FVector interest;
```
### Target Interest
```
FVector target_interest;
```
### Position
```
FVector position;
```
### Target Position
```
FVector target_position;
```
### Fixedness Multipliers
```
TArray<float> subjectFixednessMultipliers = TArray<float>{
    1.5f,
    1.32f,
    1.16f,
    1.0f
};
```
### Camera Bounds
A bounding box that contains the subjects
```
UCameraBounds* bounds;
```