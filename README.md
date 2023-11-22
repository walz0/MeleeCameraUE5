# MeleeCameraUE5
This project aims to recreate the look and feel of Melee's camera through the use of decompiled source code from the game. The original purpose of this project was only to decompile Melee's camera code, but in the process it became a learning experience for development in Unreal Engine.

---

![Showcase](/showcase.gif "Showcase")

![Bounds](/bounds.gif "Bounds")

# Overview
The camera behavior is driven by three main factors: subjects, bounds, and interest. The primary focus is on properly framing the subjects (typically players) within the scene, without sacrificing gameplay. To achieve this the camera may scale its bounds or rotate in order to keep itself in a more optimal position, avoid clipping any necessary visuals, and prevent sudden movements.


## Bounds
The camera's bounds are completely governed by the position and size of the subjects' bounding boxes. While the purpose of the bounds is to contain the subjects, the bounds may stretch and shrink depending on the direction the subjects are facing, the vertical position of the subjects, and the fixedness value of the stage. For instance, the the bottom bound may expand dramatically when there is a subject toward the upper or lower camera limit, this is to ensure that the stage is still visible for the players below them.

## Subjects
A subject is anything within the scene that the camera should keep in frame (players, NPCs, items). Subjects have their own bounding boxes that affect how the cameras bounds are drawn. The job of the camera is to draw the bounds such that all of the subjects are visible.

## Interest
The interest is arguably the most important part of the system. It uses the information produced by the subjects and bounds to find the ideal position for the camera to look at. The camera is always looking at the interest, as a result the interest should always be in the center of the screen.

---

# Structures

## Stage

### Fixedness
Fixedness is a scale factor that controls how much the camera will rotate in response to the position of the subjects (generally players). In general, the higher the amount of subjects in the scene the higher the fixedness value will be set.
```
float fixedness;
```
### Camera Limits
The maximum dimensions of the camera bounding box for each side of the screen. These are accompanied by depth limits that control how far the camera is allowed to 'zoom' in and out.
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

## CamSubject

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
Vertical offset of the bounding box from the focus.
```
float y_offset;
```

## MainCamera
### Interest
The point that the camera is currently looking at. The interest is constantly trying to find the most optimal spot to look in order to frame the subjects properly and avoid wasting space on screen.
```
FVector interest;
```
### Target Interest
The target point of interest that is calculated from changes in the camera bounds. This is what the interest interpolates toward to create a smooth pan / zoom.
```
FVector target_interest;
```
### Position
The current position of the camera in the world.
```
FVector position;
```
### Target Position
The target position of the camera.
```
FVector target_position;
```
### Fixedness Multipliers
Fixedness values for increasing numbers of players, value is set to `1.0` if the total players is greater than 4
```
TArray<float> subjectFixednessMultipliers = TArray<float>{
    1.5f,
    1.32f,
    1.16f,
    1.0f
};
```
### Camera Bounds
A bounding box that contains the subjects.
```
float x_min;
float y_min;
float x_max;
float y_max;
```
