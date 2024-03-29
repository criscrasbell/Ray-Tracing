#include <cmath>
#include "Rasterization.h"

/**
* @fn	template <class T> T barycentricWeighting(float w1, float w2, float w3, const T &i1, const T &i2, const T &i3)
* @brief	Computes the Barycentric weighting of three values.
* @tparam	T	Generic type parameter.
* @param	w1	The first weight.
* @param	w2	The second weight.
* @param	w3	The third weight.
* @param	i1	The first item.
* @param	i2	The second item.
* @param	i3	The third item.
* @return	The Barycentric weighting of the three items.
*/

template <class T>
T barycentricWeighting(float w1, float w2, float w3, const T &i1, const T &i2, const T &i3) {
	return w1*i1 + w2*i2 + w3*i3;
}

/**
 * @fn	void drawVerticalLine(FrameBuffer &fb, int x, int bottom, int top, const color &rgb)
 * @brief	Draw vertical line
 * @param [in,out]	fb	  	Framebuffer
 * @param 		  	x	  	The x coordinate.
 * @param 		  	bottom	Bottom.
 * @param 		  	top   	Top.
 * @param 		  	rgb   	Color.
 */

static void drawVerticalLine(FrameBuffer &fb, int x, int bottom, int top, const color &rgb) {
	const int H = fb.getWindowHeight();
	const int W = fb.getWindowWidth();

	if (bottom > top) {
		std::swap(bottom, top);
	}

	bottom = bottom < 0 ? 0 : bottom;
	top = top >= H ? H - 1 : top;
	for (int y = bottom; y <= top; y++) {
		if (inRectangle((float)x, (float)y, 0.0f, 0.0f, W-1.0f, H-1.0f))
			fb.setColor(x, y, rgb);
	}
}

/**
 * @fn	void drawHorizontalLine(FrameBuffer &fb, int y, int left, int right, const color &rgb)
 * @brief	Draw horizontal line
 * @param [in,out]	fb   	Framebuffer
 * @param 		  	y	 	The y coordinate.
 * @param 		  	left 	Left.
 * @param 		  	right	Right.
 * @param 		  	rgb  	Color.
 */

static void drawHorizontalLine(FrameBuffer &fb, int y, int left, int right, const color &rgb) {
	const int H = fb.getWindowHeight();
	const int W = fb.getWindowWidth();

	if (left > right) {
		std::swap(left, right);
	}
	left = left < 0 ? 0 : left;
	right = right >= W ? W - 1 : right;
	for (int x = left; x <= right; x++) {
		if (inRectangle((float)x, (float)y, 0.0f, 0.0f, W - 1.0f, H - 1.0f))
			fb.setColor(x, y, rgb);
	}
}

/**
 * @fn	void drawBresenhamLine(FrameBuffer &fb, const glm::vec2 &p1, const glm::vec2 &p2, const color &rgb)
 * @brief	Draw line using bresenham's algorithm
 * @param [in,out]	fb 	Framebuffer.
 * @param 		  	p1 	First point.
 * @param 		  	p2 	Second point.
 * @param 		  	rgb	Color.
 */

static void drawBresenhamLine(FrameBuffer &fb, 
						const glm::vec2 &p1, 
						const glm::vec2 &p2, const color &rgb) {
	const int H = fb.getWindowHeight();
	const int W = fb.getWindowWidth();

	int x0 = (int)p1.x;
	int y0 = (int)p1.y;
	int x1 = (int)p2.x;
	int y1 = (int)p2.y;

	int dx = std::abs(x1 - x0);
	int dy = std::abs(y1 - y0);

	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;

	int err = dx - dy;

	if (inRectangle((float)x0, (float)y0, 0.0f, 0.0f, W - 1.0f, H - 1.0f)) {
		fb.setColor(x0, y0, rgb);
	}

	while (x0 != x1 || y0 != y1) {
		int e2 = err << 1;

		if (e2 > -dy) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx) {
			err += dx;
			y0 += sy;
		}

		if (inRectangle((float)x0, (float)y0, 0.0f, 0.0f, W - 1.0f, H - 1.0f)) {
			fb.setColor(x0, y0, rgb);
		}
	}
}

/**
 * @fn	void drawLine(FrameBuffer &fb, const glm::vec2 &A, const glm::vec2 &B, const color &rgb)
 * @brief	Draw line
 * @param [in,out]	fb 	Framebuffer.
 * @param 		  	A  	First endpoint.
 * @param 		  	B  	Second endpoint.
 * @param 		  	rgb	Color.
 */

void drawLine(FrameBuffer &fb, const glm::vec2 &A, const glm::vec2 &B, const color &rgb) {
	if (A.x == B.x) {
		drawVerticalLine(fb, (int)A.x, (int)A.y, (int)B.y, rgb);
	} else if (A.y == B.y) {
		drawHorizontalLine(fb, (int)A.y, (int)A.x, (int)B.x, rgb);
	} else {
		drawBresenhamLine(fb, A.xy, B.xy, rgb);
	}
}

/**
 * @fn	void drawLine(FrameBuffer &frameBuffer, int x1, int y1, int x2, int y2, const color &C)
 * @brief	Draw line from (x1, y1) to (x2, y2)
 * @param [in,out]	frameBuffer	Framebuffer.
 * @param 		  	x1
 * @param 		  	y1
 * @param 		  	x2
 * @param 		  	y2
 * @param 		  	C		   	The color
 */

void drawLine(FrameBuffer &frameBuffer, int x1, int y1, int x2, int y2, const color &C) {
	drawLine(frameBuffer, glm::vec2(x1, y1), glm::vec2(x2, y2), C);
}

/**
 * @fn	void drawArc(FrameBuffer &fb, const glm::vec2 &center, float R, float startRads, float lengthInRads, const color &rgb)
 * @brief	Draw arc on xy plane, using traditional x-y axis orientation. Zero degrees is
 * 			+x axis, 90 degrees, is +y axis, etc.
 * @param [in,out]	fb				Framebuffer.
 * @param 		  	center			The center of the "circle".
 * @param 		  	R				Radius.
 * @param 		  	startRads   	The start of the arc, in radians.
 * @param 		  	lengthInRads	The length of the arc, in radians.
 * @param 		  	rgb				Color.
 */

void drawArc(FrameBuffer &fb, const glm::vec2 &center, float R,
					float startRads, float lengthInRads, const color &rgb) {
	float lengthInPixels = 2 * M_PI * R;
	int DIV = (int)std::fmax((lengthInPixels / 5), 4);
	float radInc = lengthInRads / DIV;
	for (int i = 0; i < DIV; i++) {
		float a1 = startRads + i * radInc;
		float a2 = a1 + radInc;
		glm::vec2 pt1 = pointOnCircle(center, R, a1);
		glm::vec2 pt2 = pointOnCircle(center, R, a2);
		drawLine(fb, pt1, pt2, rgb);
	}
}

/**
 * @fn	void drawWirePolygon(FrameBuffer &fb, const std::vector<glm::vec3> &pts, const color &rgb)
 * @brief	Draw wire polygon
 * @param [in,out]	fb 	Framebuffere.
 * @param 		  	pts	The vertices of the polygon.
 * @param 		  	rgb	Color.
 */

void drawWirePolygon(FrameBuffer &fb, const std::vector<glm::vec3> &pts, const color &rgb) {
	for (unsigned int i = 1; i < pts.size(); i++) {
		drawLine(fb, pts[i - 1].xy, pts[i].xy, rgb);
	}
	drawLine(fb, pts[0].xy, pts[pts.size() - 1].xy, rgb);
}

/**
 * @fn	void drawAxisOnWindow(FrameBuffer &fb)
 * @brief	Draw the x & y axis on window, centered on window
 * @param [in,out]	fb	Framebuffer.
 */

void drawAxisOnWindow(FrameBuffer &fb) {
	int W = fb.getWindowWidth();
	int H = fb.getWindowHeight();
	int W2 = W / 2;
	int H2 = H / 2;
	drawHorizontalLine(fb, H2, 0, W-1, red);
	drawVerticalLine(fb, W2, 0, H-1, green);
}

/**
 * @fn	float cheapNonPerspectiveCorrectInterpolationForLines(const glm::vec2 &start, const glm::vec2 &end, const glm::vec2 &online)
 * @brief	Cheap non perspective correct interpolation for lines
 * @param	start 	The starting vertex.
 * @param	end   	The ending vertex
 * @param	online	The 2D point to interpolate.
 * @return	Interpolated value.
 */

static float cheapNonPerspectiveCorrectInterpolationForLines(const glm::vec2 &start, const glm::vec2 &end, const glm::vec2 &online) {
	return glm::length(online - start) / glm::length(end - start);
}

/**
 * @fn	void drawVerticalLine(FrameBuffer &frameBuffer, const glm::vec3 &eyePos, const std::vector<LightSourcePtr> &lights, VertexData v0, VertexData v1, const glm::mat4 &viewingMatrix)
 * @brief	Draw vertical line
 * @param [in,out]	frameBuffer  	Framebuffer.
 * @param 		  	eyePos		 	Eye position.
 * @param 		  	lights		 	Vector of lights in scene.
 * @param 		  	v0			 	The first endpoint.
 * @param 		  	v1			 	The second endpoint.
 * @param 		  	viewingMatrix	The viewing matrix.
 */

static void drawVerticalLine(FrameBuffer &frameBuffer, const glm::vec3 &eyePos,
						const std::vector<LightSourcePtr> &lights, VertexData v0, VertexData v1,
						const glm::mat4 &viewingMatrix) {
	if (v1.position.y < v0.position.y) {
		std::swap(v0, v1);
	}

	float zDifference = v1.position.z - v0.position.z;

	for (float y = v0.position.y; y < v1.position.y; y++) {
		// Interpolate vertex attributes
		float weight = cheapNonPerspectiveCorrectInterpolationForLines(v0.position.xy, v1.position.xy, glm::vec2(v0.position.x, y));

		Fragment fragment;

		// Interpolate vertex attributes using alpha, beta, and gamma weights
		float oneMinusW = 1.0f - weight;
		fragment.material = weightedAverage(oneMinusW, v0.material, weight, v1.material);
		float z = weightedAverage(oneMinusW, v0.position.z, weight, v1.position.z);
		fragment.worldNormal = weightedAverage(oneMinusW, v0.normal, weight, v1.normal);
		fragment.worldPosition = weightedAverage(oneMinusW, v0.worldPosition, weight, v1.worldPosition);
		fragment.windowPosition = glm::vec3(v0.position.x, y, z);

		FragmentOps::processFragment(frameBuffer, eyePos, lights, fragment, viewingMatrix);
	}
}

/**
 * @fn	static void drawHorizontalLine(FrameBuffer &frameBuffer, const glm::vec3 &eyePos, const std::vector<LightSourcePtr> &lights, VertexData v0, VertexData v1, const glm::mat4 &viewingMatrix)
 * @brief	Draw horizontal line
 * @param [in,out]	frameBuffer  	Framebuffer.
 * @param 		  	eyePos		 	Eye position.
 * @param 		  	lights		 	Vector of lights in scene.
 * @param 		  	v0			 	The first endpoint.
 * @param 		  	v1			 	The second endpoint.
 * @param 		  	viewingMatrix	The viewing matrix.
 */

static void drawHorizontalLine(FrameBuffer &frameBuffer, const glm::vec3 &eyePos,
					const std::vector<LightSourcePtr> &lights, VertexData v0, VertexData v1,
					const glm::mat4 &viewingMatrix) {
	if (v1.position.x < v0.position.x) {
		std::swap(v0, v1);
	}

	for (float x = v0.position.x; x < v1.position.x; x++) {
		// Interpolate vertex attributes
		float weight = cheapNonPerspectiveCorrectInterpolationForLines(v0.position.xy, v1.position.xy, glm::vec2(x, v0.position.y));

		Fragment fragment;

		// Interpolate vertex attributes using alpha, beta, and gamma weights
		fragment.material = weightedAverage(1.0f - weight, v0.material, weight, v1.material);
		float z = weightedAverage(1 - weight, v0.position.z, weight, v1.position.z);
		fragment.worldNormal = weightedAverage(1.0f - weight, v0.normal, weight, v1.normal);
		fragment.worldPosition = weightedAverage(1.0f - weight, v0.worldPosition, weight, v1.worldPosition);
		fragment.windowPosition = glm::vec3(x, v1.position.y, z);

		FragmentOps::processFragment(frameBuffer, eyePos, lights, fragment, viewingMatrix);
	}
}

/**
 * @fn	static void midPointLine(FrameBuffer &frameBuffer, const glm::vec3 &eyePos, const std::vector<LightSourcePtr> &lights, VertexData v0, VertexData v1, const glm::mat4 &viewingMatrix)
 * @brief	Middle point line
 * @param [in,out]	frameBuffer  	Framebuffer.
 * @param 		  	eyePos		 	Eye position.
 * @param 		  	lights		 	Vector of lights in scene.
 * @param 		  	v0			 	The first endpoint.
 * @param 		  	v1			 	The second endpoint.
 * @param 		  	viewingMatrix	The viewing matrix.
 */

static void midPointLine(FrameBuffer &frameBuffer, const glm::vec3 &eyePos, 
					const std::vector<LightSourcePtr> &lights, VertexData v0, VertexData v1,
					const glm::mat4 &viewingMatrix) {
	if (v1.position.x < v0.position.x) {
		std::swap(v0, v1);
	}

	// Calculate slope of the line
	float m = (v1.position.y - v0.position.y) / (v1.position.x - v0.position.x);

	if (m > 0 && m < 1.0f) { // For slope in (0,1] More "run" than "rise"
		float y = v0.position.y;

		for (float x = v0.position.x; x < v1.position.x; x += 1.0f) {

			// Interpolate vertex attributes
			float weight = cheapNonPerspectiveCorrectInterpolationForLines(v0.position.xy, v1.position.xy, glm::vec2(x, y));

			Fragment fragment;

			// Interpolate vertex attributes using alpha, beta, and gamma weights
			fragment.material = weightedAverage(1.0f - weight, v0.material, weight, v1.material);
			float z = weightedAverage(1 - weight, v0.position.z, weight, v1.position.z);
			fragment.worldNormal = weightedAverage(1.0f - weight, v0.normal, weight, v1.normal);
			fragment.worldPosition = weightedAverage(1.0f - weight, v0.worldPosition, weight, v1.worldPosition);
			fragment.windowPosition = glm::vec3(x, y, z);

			FragmentOps::processFragment(frameBuffer, eyePos, lights, fragment, viewingMatrix);

			// Evaluate the implicit equation for the line to determine if
			// the line will be above the midpoint between the pixel centers.
			float fXY = (v0.position.y - v1.position.y) * (x + 1.0f) +
				(v1.position.x - v0.position.x) * (y + 0.5f) +
				v0.position.x * v1.position.y - v1.position.x * v0.position.y;

			if (fXY < 0) {
				y += 1.0f;
			}
		}
	} else if (m > 1) { // For slope in (1,infinity] More "run" than "rise"
		float x = v0.position.x;

		for (float y = v0.position.y; y < v1.position.y; y += 1.0f) {

			// Interpolate vertex attributes
			float weight = cheapNonPerspectiveCorrectInterpolationForLines(v0.position.xy, v1.position.xy, glm::vec2(x, y));

			Fragment fragment;

			// Interpolate vertex attributes using alpha, beta, and gamma weights
			fragment.material = weightedAverage(1.0f - weight, v0.material, weight, v1.material);
			float z = weightedAverage(1 - weight, v0.position.z, weight, v1.position.z);
			fragment.worldNormal = weightedAverage(1.0f - weight, v0.normal, weight, v1.normal);
			fragment.worldPosition = weightedAverage(1.0f - weight, v0.worldPosition, weight, v1.worldPosition);
			fragment.windowPosition = glm::vec3(x, y, z);

			FragmentOps::processFragment(frameBuffer, eyePos, lights, fragment, viewingMatrix);

			// Evaluate the implicit equation for the line to determine if
			// the line will be left or right the midpoint between the pixel centers.
			float fXY = (v0.position.y - v1.position.y) * (x + 0.5f) +
				(v1.position.x - v0.position.x) * (y + 1.0f) +
				v0.position.x * v1.position.y - v1.position.x * v0.position.y;

			if (fXY > 0) {
				x += 1.0f;
			}
		}
	} else if (m >= -1.0f && m < 0) { // For slope in [-1,0) More "run" than "rise"
		float y = v0.position.y;
		float x = v0.position.x;

		for (float x = v0.position.x; x < v1.position.x; x += 1.0f) {
			// Interpolate vertex attributes
			float weight = cheapNonPerspectiveCorrectInterpolationForLines(v0.position.xy, v1.position.xy, glm::vec2(x, y));

			Fragment fragment;

			// Interpolate vertex attributes using alpha, beta, and gamma weights
			fragment.material = weightedAverage(1.0f - weight, v0.material, weight, v1.material);
			float z = weightedAverage(1 - weight, v0.position.z, weight, v1.position.z);
			fragment.worldNormal = weightedAverage(1.0f - weight, v0.normal, weight, v1.normal);
			fragment.worldPosition = weightedAverage(1.0f - weight, v0.worldPosition, weight, v1.worldPosition);
			fragment.windowPosition = glm::vec3(x, y, z);

			FragmentOps::processFragment(frameBuffer, eyePos, lights, fragment, viewingMatrix);

			// Evaluate the implicit equation for the line to determine if
			// the line will be below the midpoint between the pixel centers.
			float fXY = (v0.position.y - v1.position.y) * (x + 1.0f) +
				(v1.position.x - v0.position.x) * (y - 0.5f) +
				v0.position.x * v1.position.y - v1.position.x * v0.position.y;

			if (fXY > 0) {

				y -= 1.0f;
			}
		}
	} else if (m < -1) { // For slope in [-infinity,-1) More "run" than "rise"
		float x = v0.position.x;

		for (float y = v0.position.y; y > v1.position.y; y -= 1.0f) {

			// Interpolate vertex attributes
			float weight = cheapNonPerspectiveCorrectInterpolationForLines(v0.position.xy, v1.position.xy, glm::vec2(x, y));

			Fragment fragment;

			// Interpolate vertex attributes using alpha, beta, and gamma weights
			fragment.material = weightedAverage(1.0f - weight, v0.material, weight, v1.material);
			float z = weightedAverage(1 - weight, v0.position.z, weight, v1.position.z);
			fragment.worldNormal = weightedAverage(1.0f - weight, v0.normal, weight, v1.normal);
			fragment.worldPosition = weightedAverage(1.0f - weight, v0.worldPosition, weight, v1.worldPosition);
			fragment.windowPosition = glm::vec3(x, y, z);

			FragmentOps::processFragment(frameBuffer, eyePos, lights, fragment, viewingMatrix);

			// Evaluate the implicit equation for the line to determine if
			// the line will be left or right the midpoint between the pixel centers.
			float fXY = (v0.position.y - v1.position.y) * (x + 0.5f) +
				(v1.position.x - v0.position.x) * (y - 1.0f) +
				v0.position.x * v1.position.y - v1.position.x * v0.position.y;

			if (fXY < 0) {
				x += 1.0f;
			}
		}
	}
}

/**
 * @fn	void drawLine(FrameBuffer &frameBuffer, const glm::vec3 &eyePos, const std::vector<LightSourcePtr> &lights, const VertexData &v0, const VertexData &v1, const glm::mat4 &viewingMatrix)
 * @brief	Draw line
 * @param [in,out]	frameBuffer  	Framebuffer.
 * @param 		  	eyePos		 	Eye position.
 * @param 		  	lights		 	Vector of lights in scene.
 * @param 		  	v0			 	The first endpoint.
 * @param 		  	v1			 	The second endpoint.
 * @param 		  	viewingMatrix	The viewing matrix.
 */

void drawLine(FrameBuffer &frameBuffer, const glm::vec3 &eyePos, 
				const std::vector<LightSourcePtr> &lights, 
				const VertexData &v0, const VertexData &v1,
					const glm::mat4 &viewingMatrix) {
	if (v0.position.x == v1.position.x) {
		drawVerticalLine(frameBuffer, eyePos, lights, v0, v1, viewingMatrix);
	} else if (v0.position.y == v1.position.y) {
		drawHorizontalLine(frameBuffer, eyePos, lights, v0, v1, viewingMatrix);
	} else {
		midPointLine(frameBuffer, eyePos, lights, v0, v1, viewingMatrix);
	}
}

/**
 * @fn	void drawManyLines(FrameBuffer &frameBuffer, const glm::vec3 &eyePos, const std::vector<LightSourcePtr> &lights, const std::vector<VertexData> &vertices, const glm::mat4 &viewingMatrix)
 * @brief	Draw many lines
 * @param [in,out]	frameBuffer  	Framebuffer.
 * @param 		  	eyePos		 	Eye position.
 * @param 		  	lights		 	Vector of lights in scene.
 * @param 		  	vertices	 	Vector of vertice-pairs.
 * @param 		  	viewingMatrix	Viewing matrix.
 */

void drawManyLines(FrameBuffer &frameBuffer, const glm::vec3 &eyePos,
					const std::vector<LightSourcePtr> &lights, 
					const std::vector<VertexData> &vertices,
					const glm::mat4 &viewingMatrix) {
	for (unsigned int i = 0; (i + 1) < vertices.size(); i += 2) {
		drawLine(frameBuffer, eyePos, lights, vertices[i], vertices[i + 1], viewingMatrix);
	}
}

/**
 * @fn	void drawWireFrameTriangle(FrameBuffer &frameBuffer, const glm::vec3 &eyePos, const std::vector<LightSourcePtr> &lights, const VertexData &v0, const VertexData &v1, const VertexData &v2, const glm::mat4 &viewingMatrix)
 * @brief	Draw wire frame triangle.
 * @param [in,out]	frameBuffer  	Framebuffer.
 * @param 		  	eyePos		 	Eye position.
 * @param 		  	lights		 	Vector of lights in scene.
 * @param 		  	v0			 	First VertexData.
 * @param 		  	v1			 	Second VertexData.
 * @param 		  	v2			 	Third VertexData.
 * @param 		  	viewingMatrix	Viewing matrix.
 */

void drawWireFrameTriangle(FrameBuffer &frameBuffer,
							const glm::vec3 &eyePos,
							const std::vector<LightSourcePtr> &lights, 
							const VertexData &v0, 
							const VertexData &v1, 
							const VertexData &v2,
							const glm::mat4 &viewingMatrix) {
	drawLine(frameBuffer, eyePos, lights, v0, v1, viewingMatrix);
	drawLine(frameBuffer, eyePos, lights, v1, v2, viewingMatrix);
	drawLine(frameBuffer, eyePos, lights, v2, v0, viewingMatrix);
}

/**
 * @fn	void drawManyWireFrameTriangles(FrameBuffer &frameBuffer, const glm::vec3 &eyePos, const std::vector<LightSourcePtr> &lights, const std::vector<VertexData> &vertices, const glm::mat4 &viewingMatrix)
 * @brief	Draw many wire frame triangles
 * @param [in,out]	frameBuffer  	Framebuffer.
 * @param 		  	eyePos		 	Eye position.
 * @param 		  	lights		 	Vector of lights in scene.
 * @param 		  	vertices	 	The vector of vertex-triplets.
 * @param 		  	viewingMatrix	Viewing matrix.
 */

void drawManyWireFrameTriangles(FrameBuffer &frameBuffer, 
									const glm::vec3 &eyePos,
									const std::vector<LightSourcePtr> &lights,
									const std::vector<VertexData> &vertices,
									const glm::mat4 &viewingMatrix) {
	for (unsigned int i = 0; (i + 2) < vertices.size(); i += 3) {
		drawWireFrameTriangle(frameBuffer, eyePos, lights, 
								vertices[i], vertices[i + 1], vertices[i + 2], viewingMatrix);
	}
}

/**
 * @fn	static inline float f01(const VertexData &v0, const VertexData &v1, const VertexData &v2, float x, float y)
 * @brief	Computes f01. Implicit equation for the line between v0 and v1.
 * @see		*** page in textbook.
 * @param	v0	v0.
 * @param	v1	v1.
 * @param	v2	v2.
 * @param	x 	x coordinate.
 * @param	y 	y coordinate.
 * @return	f01.
 */

static inline float f01(const VertexData &v0, const VertexData &v1, const VertexData &v2, float x, float y) {
	return (v0.position.y - v1.position.y) * x + (v1.position.x - v0.position.x) * y +
		(v0.position.x * v1.position.y) - (v1.position.x * v0.position.y);

}

/**
 * @fn	static inline float f12(const VertexData &v0, const VertexData &v1, const VertexData &v2, float x, float y)
 * @brief	Computes f12. Implicit equation for the line between v1 and v2.
 * @see		*** page in textbook.
 * @param	v0	v0.
 * @param	v1	v1.
 * @param	v2	v2.
 * @param	x 	x coordinate.
 * @param	y 	y coordinate.
 * @return	f12.
 */

static inline float f12(const VertexData &v0, const VertexData &v1, const VertexData &v2, float x, float y) {
	return (v1.position.y - v2.position.y) * x + (v2.position.x - v1.position.x) * y +
		(v1.position.x * v2.position.y) - (v2.position.x * v1.position.y);

}

/**
 * @fn	static inline float f20(const VertexData &v0, const VertexData &v1, const VertexData &v2, float x, float y)
 * @brief	Computes f20. Implicit equation for the line between v2 and v0.
 * @see		*** page in textbook.
 * @param	v0	v0.
 * @param	v1	v1.
 * @param	v2	v2.
 * @param	x 	x coordinate.
 * @param	y 	y coordinate.
 * @return	f02.
 */

static inline float f20(const VertexData &v0, const VertexData &v1, const VertexData &v2, float x, float y) {
	return (v2.position.y - v0.position.y) * x + (v0.position.x - v2.position.x) * y +
		(v2.position.x * v0.position.y) - (v0.position.x * v2.position.y);

}

/**
 * @fn	void drawFilledTriangle(FrameBuffer &frameBuffer, const glm::vec3 &eyePos, const std::vector<LightSourcePtr> &lights, const VertexData &v0, const VertexData &v1, const VertexData &v2, const glm::mat4 &viewingMatrix)
 * @brief	Draw filled triangle.
 * @param [in,out]	frameBuffer  	Framebuffer.
 * @param 		  	eyePos		 	Eye position.
 * @param 		  	lights		 	Vector of lights in scene.
 * @param 		  	v0			 	v0.
 * @param 		  	v1			 	v1.
 * @param 		  	v2			 	v2.
 * @param 		  	viewingMatrix	Viewing matrix.
 */

void drawFilledTriangle(FrameBuffer &frameBuffer, const glm::vec3 &eyePos, const std::vector<LightSourcePtr> &lights,
						const VertexData &v0, const VertexData &v1, const VertexData &v2,
						const glm::mat4 &viewingMatrix) {
	// Find minimimum and maximum x and y limits for the triangle
	float xMin = glm::floor(min(v0.position.x, v1.position.x, v2.position.x));
	float xMax = glm::ceil(max(v0.position.x, v1.position.x, v2.position.x));
	float yMin = glm::floor(min(v0.position.y, v1.position.y, v2.position.y));
	float yMax = glm::ceil(max(v0.position.y, v1.position.y, v2.position.y));

	float fAlpha = f12(v0, v1, v2, v0.position.x, v0.position.y);
	float fBeta = f20(v0, v1, v2, v1.position.x, v1.position.y);
	float fGamma = f01(v0, v1, v2, v2.position.x, v2.position.y);

	for (float y = yMin; y <= yMax; y++) {
		for (float x = xMin; x <= xMax; x++) {	
			// Calculate the weights for Gouraud inperpolation
			// If any weight is negative, the fragment is not in the triangle
			float alpha = f12(v0, v1, v2, x, y) / fAlpha;
			float beta = f20(v0, v1, v2, x, y) / fBeta;
			float gamma = f01(v0, v1, v2, x, y) / fGamma;

			// Determine if the pixel position is inside the triangle
			if (alpha >= 0 && beta >= 0 && gamma >= 0) {
				if ((alpha > 0 || fAlpha * f12(v0, v1, v2, -1, -1) > 0) &&
					(beta > 0 || fBeta * f20(v0, v1, v2, -1, -1) > 0) &&
					(gamma > 0 || fGamma * f01(v0, v1, v2, -1, -1) > 0)) {

					Fragment fragment;

					// Interpolate vertex attributes using alpha, beta, and gamma weights
					fragment.material = barycentricWeighting(alpha, beta, gamma,
															v0.material, v1.material, v2.material);
					float z = barycentricWeighting(alpha, beta, gamma,
													v0.position.z, v1.position.z, v2.position.z);
					fragment.windowPosition = glm::vec3(x, y, z);
					fragment.worldNormal = barycentricWeighting(alpha, beta, gamma,
																v0.normal, v1.normal, v2.normal);
					fragment.worldPosition = barycentricWeighting(alpha, beta, gamma,
																v0.worldPosition, v1.worldPosition, v2.worldPosition);
					FragmentOps::processFragment(frameBuffer, eyePos, lights, fragment, viewingMatrix);
				}
			}
		}
	}
}

/**
 * @fn	void drawManyFilledTriangles(FrameBuffer &frameBuffer, const glm::vec3 &eyePos, const std::vector<LightSourcePtr> &lights, const std::vector<VertexData> &vertices, const glm::mat4 &viewingMatrix)
 * @brief	Draw many filled triangles,
 * @param [in,out]	frameBuffer  	Framebuffer.
 * @param 		  	eyePos		 	Eye position.
 * @param 		  	lights		 	Vector of lights in scene.
 * @param 		  	vertices	 	The vector of vertice-triplets.
 * @param 		  	viewingMatrix	Viewing matrix.
 */

void drawManyFilledTriangles(FrameBuffer &frameBuffer, const glm::vec3 &eyePos, 
							const std::vector<LightSourcePtr> &lights, const std::vector<VertexData> &vertices,
							const glm::mat4 &viewingMatrix) {
	for (int i = 0; i < (int)vertices.size() - 2; i += 3) {
		const VertexData &Vi = vertices[i];
		const VertexData &Vi1 = vertices[i+1];
		const VertexData &Vi2 = vertices[i+2];
		drawFilledTriangle(frameBuffer, eyePos, lights, Vi, Vi1, Vi2, viewingMatrix);
	}
}