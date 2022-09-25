#include "rlut.hpp"

//
// --- RANDOM ---
//

void RNG::init()
{
	seed(getTimeSinceEpoch());
}

void RNG::seed(const unsigned int seedVal)
{
	m_engine.seed(seedVal);
}

int RNG::randRange(const int minVal, const int maxVal)
{
	return std::uniform_int_distribution<int>(minVal, maxVal)(m_engine);
}

float RNG::randRange(const float minVal, const float maxVal)
{
	return std::uniform_real_distribution<float>(minVal, maxVal)(m_engine);
}

int RNG::randOffset(const int center, const int offset)
{
	return randRange(center - offset, center + offset);
}

float RNG::randOffset(const float center, const float offset)
{
	return randRange(center - offset, center + offset);
}

bool RNG::randGaussian(const float mean, const float stddev)
{
	return std::normal_distribution<float>(mean, stddev)(m_engine);
}

bool RNG::randBernoulli(const double successRate)
{
	return std::bernoulli_distribution(successRate)(m_engine);
}

Vector2 RNG::randPointInLine(const Vector2 start, const Vector2 end)
{
	return Vector2Lerp(start, end, randRange(0.0f, 1.0f));
}

Vector2 RNG::randPointInRect(const Rectangle rect)
{
	return Vector2{ randRange(rect.x, rect.x + rect.width), randRange(rect.y, rect.y + rect.height) };
}

Vector2 RNG::randPointInCircle(const float minRadius, const float maxRadius, const float minAngle, const float maxAngle)
{
	return vec2Create(maxRadius * std::sqrt(randRange(minRadius / maxRadius, 1.0f)), randRange(minAngle, maxAngle));
}

std::mt19937& RNG::getEngine()
{
	return m_engine;
}

//
// --- NOISE ---
//

void NoiseGenerator::scramble(RNG& rng, const float minVal, const float maxVal, const int gridLength, const int gridHeight)
{
	const int len = gridLength * gridHeight;
	m_samples.resize((std::size_t)len);
	std::uniform_real_distribution<float> d(minVal, maxVal);
	std::mt19937& mt = rng.getEngine();
	for (auto& x : m_samples)
	{
		x = d(mt);
	}
}

const float NoiseGenerator::getValue(const float t) const
{
	const std::size_t a = (std::size_t)std::floor(t) % m_samples.size();
	const std::size_t b = (a + 1) % m_samples.size();
	return Lerp(m_samples[a], m_samples[b], std::fmod(t, 1.0f));
}

const float NoiseGenerator::getValue(const Vector2 t) const
{
	return 0.0f;
}

//
// --- TIMING ---
//

void Timer::restart()
{
	m_start = std::chrono::steady_clock::now();
}

const float Timer::getTime() const
{
	return std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1>>>(std::chrono::steady_clock::now() - m_start).count();
}

unsigned int getTimeSinceEpoch()
{
	return std::chrono::duration_cast<std::chrono::duration<unsigned int, std::milli>>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

//
// --- WINDOW ---
//

Vector2 getWindowSize()
{
	return Vector2{ static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) };
}

//
// --- DRAW ---
//

void drawTextCentered(const Font font, const char* text, const Vector2 position, const float fontSize, const float spacing, const Color color)
{
	const Vector2 tSize = MeasureTextEx(font, text, fontSize, spacing);
	DrawTextEx(font, text, Vector2{ position.x - 0.5f * tSize.x, position.y - 0.5f * tSize.y }, fontSize, spacing, color);
}

void drawRectCentered(const Rectangle rect, const Vector2 position, const Color color)
{
	DrawRectangleRec(Rectangle{ rect.x - 0.5f * rect.width, rect.y - 0.5f * rect.height, rect.width, rect.height }, color);
}

//
// --- MATH ---
//

Vector2 vec2Create(const float length, const float angle)
{
	const float a = DEG2RAD * angle;
	return Vector2{ length * std::cos(a), length * std::sin(a) };
}

Vector2 vec2Clamp(const Vector2 v, const Rectangle rect)
{
	return Vector2{ Clamp(v.x, rect.x, rect.x + rect.width), Clamp(v.y, rect.y, rect.y + rect.height) };
}

float oscillate(const float minVal, const float maxVal, const float frequency, const float phase, const float t)
{
	return Lerp(minVal, maxVal, 0.5f * (1.0f - std::cos(2.0f * PI * frequency * t + phase)));
}

float reverseLerp(const float start, const float end, const float point)
{
	return (point - start) / (end - start);
}

//
// --- PHYSICS ---
//

Vector2 calculateGravitationalAcceleration(const Vector2 sourcePosition, const float targetMass, const Vector2 targetPosition, const float gravitationalConstant)
{
	const Vector2 r = Vector2Subtract(targetPosition, sourcePosition);
	const float rlen = Vector2LengthSqr(r);
	return Vector2Scale(Vector2Normalize(r), (gravitationalConstant * targetMass) / rlen);
}

Vector2 calculateCoulombForceAcceleration(const float sourceMass, const float sourceCharge, const Vector2 sourcePosition, const float targetCharge, const Vector2 targetPosition, const float electrostaticConstant)
{
	const Vector2 r = Vector2Subtract(targetPosition, sourcePosition);
	const float rlen = Vector2LengthSqr(r);
	return Vector2Scale(Vector2Normalize(r), (electrostaticConstant * sourceCharge * targetCharge) / (sourceMass * rlen));
}

Vector2 calculateMotion(const Vector2 v0, const Vector2 v, const float dt)
{
	return Vector2{ v0.x + v.x * dt, v0.y + v.y * dt };
}

Vector2 calculatePostCollisionVelocity(const float sourceMass, const Vector2 sourceVelocity, const float targetMass, const Vector2 targetVelocity, const Vector2 normal, const float restitution)
{
	const Vector2 vrel = Vector2Subtract(sourceVelocity, targetVelocity);
	const Vector2 n = Vector2Normalize(normal);
	const float j = (-(1.0f + restitution) * Vector2DotProduct(vrel, n)) / ((1.0f / sourceMass) + (1.0f / targetMass));
	return Vector2Add(sourceVelocity, Vector2Scale(n, j / sourceMass));
}

//
// --- CONTROLLER ---
//

Vector2 isDirectionKeyDown(const float length, const KeyboardKey up, const KeyboardKey down, const KeyboardKey left, const KeyboardKey right)
{
	Vector2 v{ 0.0f, 0.0f };
	if (IsKeyDown(up)) { v.y += length; }
	if (IsKeyDown(down)) { v.y -= length; }
	if (IsKeyDown(left)) { v.x -= length; }
	if (IsKeyDown(right)) { v.x += length; }
	return v;
}

Vector2 isDirectionKeyPressed(const float length, const KeyboardKey up, const KeyboardKey down, const KeyboardKey left, const KeyboardKey right)
{
	Vector2 v{ 0.0f, 0.0f };
	if (IsKeyPressed(up)) { v.y += length; }
	if (IsKeyPressed(down)) { v.y -= length; }
	if (IsKeyPressed(left)) { v.x -= length; }
	if (IsKeyPressed(right)) { v.x += length; }
	return v;
}

int isDirectionKeyDown(const int value, const KeyboardKey up, const KeyboardKey down)
{
	int v = 0;
	if (IsKeyDown(up)) { v += value; }
	if (IsKeyDown(down)) { v -= value; }
	return v;
}

int isDirectionKeyPressed(const int value, const KeyboardKey up, const KeyboardKey down)
{
	int v = 0;
	if (IsKeyPressed(up)) { v += value; }
	if (IsKeyPressed(down)) { v -= value; }
	return v;
}

float isDirectionKeyDown(const float value, const KeyboardKey up, const KeyboardKey down)
{
	float v = 0.0f;
	if (IsKeyDown(up)) { v += value; }
	if (IsKeyDown(down)) { v -= value; }
	return v;
}

float isDirectionKeyPressed(const float value, const KeyboardKey up, const KeyboardKey down)
{
	float v = 0.0f;
	if (IsKeyPressed(up)) { v += value; }
	if (IsKeyPressed(down)) { v -= value; }
	return v;
}
