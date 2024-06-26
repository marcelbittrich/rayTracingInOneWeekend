#pragma once

#include "../tools/rtweekend.h"
#include "../tools/ray.h"
#include "../tools/color.h"

struct HitRecord;

class Material
{
public:
	virtual ~Material() = default;

	virtual bool Scatter(const Ray& ray, const HitRecord& rec, color& attenuation, Ray& scattered) const = 0;
	virtual color Emitted() const
	{
		return color(0.0, 0.0, 0.0);
	}
	virtual color GetColor() = 0;
	virtual void SetColor(color newColor) = 0;
};

class Lambertian : public Material
{
public:
	Lambertian(const color& albedo) : m_albedo(albedo) {}

	bool Scatter(const Ray& ray, const HitRecord& rec, color& attenuation, Ray& scattered) const override;
	color GetColor() override { return m_albedo; };
	void SetColor(color newColor) override { m_albedo = newColor; };

private:
	color m_albedo;
};

class Metal : public Material
{
public:
	Metal(const color& albedo, double fuzz) 
		: m_albedo(albedo), m_fuzz(fuzz < 1 ? fuzz : 1) {}

	bool Scatter(const Ray& ray, const HitRecord& rec, color& attenuation, Ray& scattered) const override;
	color GetColor() override { return m_albedo; };
	void SetColor(color newColor) override { m_albedo = newColor; };

private:
	color m_albedo;
	double m_fuzz;
};

class Dielectric : public Material
{
public:
	Dielectric(const color& albedo, double refrectionIndex)
		: m_albedo(albedo), m_refractionIndex(refrectionIndex){}

	bool Scatter(const Ray& ray, const HitRecord& rec, color& attenuation, Ray& scattered) const override;
	color GetColor() override { return m_albedo; };
	void SetColor(color newColor) override { m_albedo = newColor; };

private:
	color m_albedo;
	double m_refractionIndex;
	double reflectance(double cosine, double refractionIndex) const;
};


class DiffuseLight : public Material
{
public:
	DiffuseLight(color color, double brightness)
		: m_lightColor(color), m_brightness(brightness) {};
	bool Scatter(const Ray& ray, const HitRecord& rec, color& attenuation, Ray& scattered) const override 
	{
		return false;
	}

	color Emitted() const
	{
		return m_lightColor * m_brightness;
	}

	color GetColor() override { return m_lightColor; };
	void SetColor(color newColor) override { m_lightColor = newColor; };

private:
	color m_lightColor;
	double m_brightness;
};