#pragma once

#include <raylib.h>
#include <raymath.h>

#include <unordered_map>
#include <string>
#include <random>
#include <vector>
#include <chrono>
#include <cmath>

//
// --- RANDOM ---
//

class RNG
{
public:

	RNG() = default;

	void init();
	void seed(const unsigned int seedVal);

	int randRange(const int minVal, const int maxVal);
	float randRange(const float minVal, const float maxVal);
	int randOffset(const int center, const int offset);
	float randOffset(const float center, const float offset);
	bool randGaussian(const float mean, const float stddev);
	bool randBernoulli(const double successRate);

	Vector2 randPointInLine(const Vector2 start, const Vector2 end);
	Vector2 randPointInRect(const Rectangle rect);
	Vector2 randPointInCircle(const float minRadius, const float maxRadius, const float minAngle, const float maxAngle);

	std::mt19937& getEngine();

private:
	std::mt19937 m_engine;
};

//
// --- NOISE ---
//

class NoiseGenerator
{
public:

	NoiseGenerator() = default;

	void scramble(RNG& rng, const float minVal, const float maxVal, const int gridLength, const int gridHeight);

	const float getValue(const float t) const;
	const float getValue(const Vector2 t) const;

private:
	std::vector<float> m_samples;
	int m_gridLength = 0;
	int m_gridHeight = 0;
};

//
// --- TEXTURE MANAGER ---
//

class TextureManager
{
public:

	TextureManager() = default;
	~TextureManager();

	bool insertTexture(const std::string& id, const std::string& filename);
	void eraseTexture(const std::string& id);
	const Texture* getTexture(const std::string& id);
	void clear();

private:
	std::unordered_map<std::string, Texture2D> m_textures;
	std::unordered_map<std::string, std::string> m_files;
	std::string m_corePath;
};

//
// --- FONT MANAGER ---
//

//
// --- TIMING ---
//

class Timer
{
public:

	Timer() = default;

	void restart();

	const float getTime() const;

private:
	std::chrono::time_point<std::chrono::steady_clock> m_start{ std::chrono::steady_clock::now() };
};

unsigned int getTimeSinceEpoch();

//
// --- WINDOW ---
//

Vector2 getWindowSize();

//
// --- DRAW ---
//

void drawTextCentered(const Font font, const char* text, const Vector2 position, const float fontSize, const float spacing, const Color color);
void drawRectCentered(const Rectangle rect, const Vector2 position, const Color color);

//
// --- MATH ---
//

Vector2 vec2Create(const float length, const float angle);
Vector2 vec2Clamp(const Vector2 v, const Rectangle rect);
float oscillate(const float minVal, const float maxVal, const float frequency, const float phase, const float t);
float reverseLerp(const float start, const float end, const float point);

//
// --- PHYSICS ---
//

Vector2 calculateGravitationalAcceleration(const Vector2 sourcePosition, const float targetMass, const Vector2 targetPosition, const float gravitationalConstant);
Vector2 calculateCoulombForceAcceleration(const float sourceMass, const float sourceCharge, const Vector2 sourcePosition, const float targetCharge, const Vector2 targetPosition, const float electrostaticConstant);
Vector2 calculateMotion(const Vector2 v0, const Vector2 v, const float dt);
Vector2 calculatePostCollisionVelocity(const float sourceMass, const Vector2 sourceVelocity, const float targetMass, const Vector2 targetVelocity, const Vector2 normal, const float restitution);

//
// --- CONTROLLER ---
//

Vector2 isDirectionKeyDown(const float length, const KeyboardKey up, const KeyboardKey down, const KeyboardKey left, const KeyboardKey right);
Vector2 isDirectionKeyPressed(const float length, const KeyboardKey up, const KeyboardKey down, const KeyboardKey left, const KeyboardKey right);
int isDirectionKeyDown(const int value, const KeyboardKey up, const KeyboardKey down);
int isDirectionKeyPressed(const int value, const KeyboardKey up, const KeyboardKey down);
float isDirectionKeyDown(const float value, const KeyboardKey up, const KeyboardKey down);
float isDirectionKeyPressed(const float value, const KeyboardKey up, const KeyboardKey down);
