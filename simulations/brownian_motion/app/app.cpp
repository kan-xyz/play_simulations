#include "app.hpp"

#include <algorithm>

App::App()
{
    InitWindow(800, 600, "brownian motion");
    SetTargetFPS(60);
}

App::~App()
{
    CloseWindow();
}

void App::run()
{
    init();
    while (!WindowShouldClose())
    {
        update();
        BeginDrawing();
        ClearBackground(BLACK);
        render();
        EndDrawing();
    }
}

void App::init()
{
    const Vector2 wSize = getWindowSize();
    m_particles.resize(5);
    for (auto& p : m_particles)
    {
        p.radius = 20.0f;
        p.mass = 100.f;
        p.color = ORANGE;
        p.position = m_rng.randPointInRect(Rectangle{ p.radius, p.radius, wSize.x - 2.0f * p.radius, wSize.y - 2.0f * p.radius });
        p.velocity = Vector2Zero();
    }
    m_molecules.resize(500);
    for (auto& m : m_molecules)
    {
        m.radius = 3.0f;
        m.color = SKYBLUE;
        m.mass = 10.0f;
        m.velocity = vec2Create(300.0f, m_rng.randRange(0.0f, 360.0f));
        const Rectangle rect = { m.radius, m.radius, wSize.x - 2.0f * m.radius, wSize.y - 2.0f * m.radius };
        Vector2 pos = m_rng.randPointInRect(rect);
        bool success = false;
        while (!success)
        {
            success = true;
            for (auto& p : m_particles)
            {
                if (CheckCollisionPointCircle(pos, p.position, p.radius))
                {
                    pos = m_rng.randPointInRect(rect);
                    success = false;
                    break;
                }
            }
        }
        m.position = pos;
    }
}

void App::update()
{
    const float dt = GetFrameTime();
    const Vector2 wSize = getWindowSize();
    for (auto& m : m_molecules)
    {
        const Vector2 startPos = m.position;
        Vector2 finalPos = Vector2Add(startPos, Vector2Scale(m.velocity, dt));
        if (finalPos.y < 0.0f)
        {
            finalPos = Vector2Lerp(startPos, finalPos, reverseLerp(startPos.y, finalPos.y, 0.0f));
            m.velocity.y = -m.velocity.y;
        }
        else if (finalPos.y > wSize.y)
        {
            finalPos = Vector2Lerp(startPos, finalPos, reverseLerp(startPos.y, finalPos.y, wSize.y));
            m.velocity.y = -m.velocity.y;
        }
        if (finalPos.x < 0.0f)
        {
            finalPos = Vector2Lerp(startPos, finalPos, reverseLerp(startPos.x, finalPos.x, 0.0f));
            m.velocity.x = -m.velocity.x;
        }
        else if (finalPos.x > wSize.x)
        {
            finalPos = Vector2Lerp(startPos, finalPos, reverseLerp(startPos.x, finalPos.x, wSize.x));
            m.velocity.x = -m.velocity.x;
        }
        for (auto& p : m_particles)
        {
            if (CheckCollisionPointCircle(finalPos, p.position, p.radius))
            {
                const Vector2 dr = Vector2Subtract(finalPos, p.position);
                const float joltLength = p.radius - Vector2Length(dr);
                finalPos = Vector2Add(finalPos, Vector2Scale(Vector2Normalize(dr), joltLength));
                const Vector2 vp = calculatePostCollisionVelocity(p.mass, p.velocity, m.mass, m.velocity, Vector2Normalize(Vector2Subtract(finalPos, p.position)), 1.0f);
                const Vector2 vm = calculatePostCollisionVelocity(m.mass, m.velocity, p.mass, p.velocity, Vector2Normalize(Vector2Subtract(p.position, finalPos)), 1.0f);
                p.velocity = vp;
                m.velocity = vm;
            }
        }
        m.position = finalPos;
    }
    for (auto& p : m_particles)
    {
        const Vector2 startPos = p.position;
        Vector2 finalPos = Vector2Add(startPos, Vector2Scale(p.velocity, dt));
        if ((finalPos.y - p.radius) < 0.0f)
        {
            finalPos = Vector2Lerp(startPos, finalPos, reverseLerp(startPos.y, finalPos.y, p.radius));
            p.velocity.y = -p.velocity.y;
        }
        else if ((finalPos.y + p.radius) > wSize.y)
        {
            finalPos = Vector2Lerp(startPos, finalPos, reverseLerp(startPos.y, finalPos.y, wSize.y - p.radius));
            p.velocity.y = -p.velocity.y;
        }
        if ((finalPos.x - p.radius) < 0.0f)
        {
            finalPos = Vector2Lerp(startPos, finalPos, reverseLerp(startPos.x, finalPos.x, p.radius));
            p.velocity.x = -p.velocity.x;
        }
        else if ((finalPos.x + p.radius) > wSize.x)
        {
            finalPos = Vector2Lerp(startPos, finalPos, reverseLerp(startPos.x, finalPos.x, wSize.x - p.radius));
            p.velocity.x = -p.velocity.x;
        }
        p.position = finalPos;
    }
}

void App::render()
{
    for (const auto& m : m_molecules)
    {
        DrawCircleV(m.position, m.radius, m.color);
    }
    for (const auto& p : m_particles)
    {
        DrawCircleV(p.position, p.radius, p.color);
    }
}
