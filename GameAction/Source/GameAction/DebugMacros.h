#pragma once

#define DRAW_SPHERE(Location) if(GetWorld()) DrawDebugSphere(GetWorld(),Location,25.f, 12, FColor::Red, true);
#define DRAW_SPHERE_Color(Location,Color) if(GetWorld()) DrawDebugSphere(GetWorld(),Location,12.f, 12, Color, false ,5.0f);
#define DRAW_SPHERE_SingleFrame(Location) if(GetWorld()) DrawDebugSphere(GetWorld(),Location,25.f, 12, FColor::Red, false,-1.f);
#define DRAW_LINES(StartLocation,EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(),StartLocation,EndLocation,FColor::Red,true,-1.f,0,1.f);
#define DRAW_LINES_SingleFrame(StartLocation,EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(),StartLocation,EndLocation,FColor::Red,false,-1.f,0,1.f);
#define DRAW_Point(Location) if(GetWorld()) DrawDebugPoint(GetWorld(),Location,15.f,FColor::Red,true);
#define DRAW_Point_SingleFrame(Location) if(GetWorld()) DrawDebugPoint(GetWorld(),Location,15.f,FColor::Red,false,-1.f);
#define DRAW_VECTOR(StartLocation,EndLocation) if(GetWorld())\
{\
DrawDebugLine(GetWorld(),StartLocation,EndLocation,FColor::Red,true,-1.f,0,1.f);\
DrawDebugPoint(GetWorld(),Location,15.f,FColor::Red,true);\
}
#define DRAW_VECTOR_SingleFrame(StartLocation,EndLocation) if(GetWorld())\
{\
DrawDebugLine(GetWorld(),StartLocation,EndLocation,FColor::Red,false,-1.f,0,1.f);\
DrawDebugPoint(GetWorld(),Location,15.f,FColor::Red,false,-1.f);\
}