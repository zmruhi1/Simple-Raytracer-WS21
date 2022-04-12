#include <core/scalar.h>
#include <core/image.h>
#include <rt/renderer.h>
#include <rt/ray.h>
#include <iostream>
#include "cameras/perspective.h"
#include "integrators/integrator.h"

#include<fstream>

namespace rt {

	Renderer::Renderer(Camera* cam, Integrator* integrator)
	{
		mCamera = cam;
		mIntegrator = integrator;
	}

	void Renderer::render(Image& img) {
		for (uint y = 0; y < img.height(); ++y)
		{
			for (uint x = 0; x < img.width(); ++x)
			{
				float cameraX = 2.0 * float(x + 0.5) / float(img.width()) - 1;
				float cameraY = 1 - 2.0 * float(y + 0.5) / float(img.height());

				Ray r = mCamera->getPrimaryRay(cameraX, cameraY);

				RGBColor color = mIntegrator->getRadiance(r);
				
				RGBColor* pixelColor = &img(x, y);
				*pixelColor = color * 255;
			}
		}

	}
}

rt::RGBColor a1computeColor(rt::uint x, rt::uint y, rt::uint width, rt::uint height);

namespace rt {

    void Renderer::test_render1(Image& img) {
        int height = img.height();
        int width = img.width();
       

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                RGBColor cPixel = a1computeColor(j, i, width, height);
                cPixel = cPixel * 255;

                RGBColor imgPixel = RGBColor(static_cast<int>(cPixel.r), static_cast<int>(cPixel.g), static_cast<int>(cPixel.b));
                img.setPixel(i * width + j, imgPixel);
            }
        }
    }
}

rt::RGBColor a2computeColor(const rt::Ray& r);

namespace rt {

void Renderer::test_render2(Image& img) {
	Image sumImage(img);

	for (uint y = 0; y < img.height(); ++y)
	{
		for (uint x = 0; x < img.width(); ++x)
		{
			float cameraX = 2.0 * float(x + 0.5) / float(img.width()) - 1;
			float cameraY = 1 - 2.0 * float(y + 0.5) / float(img.height());

			Ray r = mCamera->getPrimaryRay(cameraX, cameraY);

			RGBColor color = a2computeColor(r);
			RGBColor* pixelColor = &img(x, y);
			*pixelColor = color * 255;

		}
	}
}

}

