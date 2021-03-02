#pragma once
#include "stdafx.h"

//정점 구조체
struct VertexP
{
    VertexP() { Position.z = 0.0f; }

    VertexP(Vector3 position)
        : Position(position)
    {
		Position.z = 0.0f;
    }

    Vector3 Position;
};

struct VertexPC
{
    VertexPC() { Position.z = 0.0f; }

    VertexPC(Vector3 position,Color  color)
        : Position(position),color(color)
    {
		Position.z = 0.0f;
    }

    Vector3 Position;
    Color   color;
};

struct VertexPT
{
    VertexPT() { Position.z = 0.0f; }

    VertexPT(Vector3 position,Vector2  uv)
        : Position(position),Uv(uv)
    {
		Position.z = 0.0f;
    }

    Vector3 Position;
    Vector2 Uv; //텍스쳐 좌표
};