#pragma once

#include "rlut/rlut.hpp"

struct Particle
{
    Vector2 position{ 0.0f, 0.0f };
    Vector2 velocity{ 0.0f, 0.0f };
    Color color = RAYWHITE;
    float radius = 0.0f;
    float mass = 0.0f;
};

class App
{
public:

    App();
    ~App();

    void run();

private:

    void init();
    void update();
    void render();

private:
    RNG m_rng;
    std::vector<Particle> m_particles;
    std::vector<Particle> m_molecules;
};
