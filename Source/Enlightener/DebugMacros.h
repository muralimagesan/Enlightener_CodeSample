#pragma once

#include "Kismet/KismetSystemLibrary.h"

#define DRAW_POINT(Location)                                if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Purple, true)
#define DRAW_POINT_SINGLEFRAME(Location)                    if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Purple, false, -1.f)
#define DRAW_SPHERE(Location)	                            if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 40.f, 24, FColor::Purple, true)
#define DRAW_SPHERE_SINGLEFRAME(Location)	                if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 40.f, 24, FColor::Purple, false, -1.f)
#define DRAW_COLOR_SPHERE(Location, Color)                  if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 10.f, 24, Color, false, 5.f)
#define DRAW_COLOR_SPHERE_SINGLEFRAME(Location, Color)      if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 40.f, 24, Color, false, -1.f)
#define DRAW_LINE(StartLocation, EndLocation)               if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Purple, true)
#define DRAW_LINE_SINGLEFRAME(StartLocation, EndLocation)   if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Purple, false, -1.f)
#define DRAW_VECTOR(StartLocation, EndLocation)             if (GetWorld()) DrawDebugDirectionalArrow(GetWorld(), StartLocation, EndLocation, 2.f, FColor::Purple, true, -1.f, 0U, 1.f)
#define DRAW_VECTOR_SINGLEFRAME(StartLocation, EndLocation) if (GetWorld()) DrawDebugDirectionalArrow(GetWorld(), StartLocation, EndLocation, 2.f, FColor::Purple, false, -1.f, 0U, 1.f)

#define DRAW_LINE_DOT(StartLocation, EndLocation)                                                       \
                                                            if (GetWorld())                             \
                                                            {                                           \
                                                                DRAW_LINE(StartLocation, EndLocation);  \
                                                                DRAW_POINT(EndLocation);                \
                                                            }

#define DRAW_LINE_DOT_SINGLEFRAME(StartLocation, EndLocation)                                                       \
                                                            if (GetWorld())                                         \
                                                            {                                                       \
                                                                DRAW_LINE_SINGLEFRAME(StartLocation, EndLocation);  \
                                                                DRAW_POINT_SINGLEFRAME(EndLocation);                \
                                                            }