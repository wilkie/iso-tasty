#include "apsis/geometry/rectangle.h"


Apsis::Geometry::Rectangle::Rectangle(float x, float y, float width, float height)
  : x(x), y(y), width(width), height(height) {
}

bool Apsis::Geometry::Rectangle::intersects(Rectangle* rectangle) const {
  bool doesNotIntersect = false;
  doesNotIntersect |= rectangle->x > (x + width);
  doesNotIntersect |= x            > (rectangle->x + rectangle->width);
  doesNotIntersect |= rectangle->y > (y + height);
  doesNotIntersect |= y            > (rectangle->y + rectangle->height);
  return !doesNotIntersect;
}

bool Apsis::Geometry::Rectangle::intersects(Line* line) const {
  float deltaX = (line->points[1].x - line->points[0].x);
  float deltaY = (line->points[1].y - line->points[0].y);

  // Liang-Barsky Algorithm to find the intersection point

  // Parametric Equation for a Line:
  // x = boundingBox.x + t * (boundingBox.width);
  // y = boundingBox.y + t * (boundingBox.height);

  // Use these with the sides of the rectangle to find values of 't'

  // If these are within the range [0..1], then the line intersects the axis of
  //   the rectangle.
  float tMin, tMax;
  tMin = 0.0;
  tMax = 1.0;

  float p[4];
  float q[4];
  float exactPosition[4];

  float halfWidth  = width / 2.0f;
  float halfHeight = height / 2.0f;

  float left   = x - halfWidth;
  float right  = x + halfWidth;
  float top    = y - halfWidth;
  float bottom = y + halfWidth;

  // Left
  p[0] = -deltaX;
  q[0] = line->points[0].x - left;
  exactPosition[0] = left;

  // Right
  p[1] = deltaX;
  q[1] = right - line->points[0].x;
  exactPosition[1] = right;

  // Top
  p[2] = -deltaY;
  q[2] = line->points[0].y - top;
  exactPosition[2] = top;

  // Bottom
  p[3] = deltaY;
  q[3] = bottom - line->points[0].y;
  exactPosition[3] = bottom;

  // Special cases (Dare I call them edge cases?)
  // Gliding past the edge
  if (p[0] == 0 && p[1] == 0) {
    if (line->points[0].x == left || line->points[0].x == right) {
      return false;
    }
  }
  else if (p[2] == 0 && p[3] == 0) {
    if (line->points[0].y == top || line->points[0].y == bottom) {
      return false;
    }
  }

  // Unix Vector Magnitudes
  float magnitudeX = deltaX;
  float magnitudeY = deltaY;
  float r;

  for (int i = 0; i < 4; i++) {
    if (p[i] < 0.0) {
      // Outside to Inside
      r = q[i] / p[i];
      if (r > tMax) {
        return false;
      }
      else if (r > tMin) {
        tMin = r;
      }
    }
    else if (p[i] > 0.0) {
      // Inside to Outside
      r = q[i] / p[i];
      if (r < tMin) {
        return false;
      }
      else if (r < tMax) {
        tMax = r;
      }
    }
    else if (p[i] == 0.0 && q[i] < 0.0) {
      // Reject
      return false;
    }
  }

  if (tMin > tMax) {
    // Outside
    return false;
  }

  // Line intersects between tMin and tMax
  return true;
}

unsigned int Apsis::Geometry::Rectangle::clip(Line* line, float* tMin, float* tMax) const {
  float deltaX = (line->points[1].x - line->points[0].x);
  float deltaY = (line->points[1].y - line->points[0].y);

  // Liang-Barsky Algorithm to find the intersection point

  // Parametric Equation for a Line:
  // x = boundingBox.x + t * (boundingBox.width);
  // y = boundingBox.y + t * (boundingBox.height);

  // Use these with the sides of the rectangle to find values of 't'

  // If these are within the range [0..1], then the line intersects the axis of
  //   the rectangle.
  *tMin = 0.0;
  Point  pMin;
  *tMax = 1.0;
  Point  pMax;

  float p[4];
  float q[4];
  float exactPosition[4];

  float halfWidth  = width  / 2.0f;
  float halfHeight = height / 2.0f;

  float left   = x - halfWidth;
  float right  = x + halfWidth;
  float top    = y - halfWidth;
  float bottom = y + halfWidth;

  // Left
  p[0] = -deltaX;
  q[0] = line->points[0].x - left;
  exactPosition[0] = left;

  // Right
  p[1] = deltaX;
  q[1] = right - line->points[0].x;
  exactPosition[1] = right;

  // Top
  p[2] = -deltaY;
  q[2] = line->points[0].y - top;
  exactPosition[2] = top;

  // Bottom
  p[3] = deltaY;
  q[3] = bottom - line->points[0].y;
  exactPosition[3] = bottom;

  // Special cases (Dare I call them edge cases?)
  // Gliding past the edge
  if (p[0] == 0 && p[1] == 0) {
    if (line->points[0].x == left || line->points[0].x == right) {
      return 0;
    }
  }
  else if (p[2] == 0 && p[3] == 0) {
    if (line->points[0].y == top || line->points[0].y == bottom) {
      return 0;
    }
  }

  // Unix Vector Magnitudes
  float magnitudeX = deltaX;
  float magnitudeY = deltaY;
  float r;

  unsigned int line_index_min = 0;
  unsigned int line_index_max = 0;

  for (int i = 0; i < 4; i++) {
    if (p[i] < 0.0) {
      // Outside to Inside
      r = q[i] / p[i];
      if (r > *tMax) {
        return 0;
      }
      else if (r > *tMin) {
        *tMin = r;
        line_index_min = i;
        if (i < 2) {
          pMin.x = exactPosition[i];
          pMin.y = line->points[0].y + *tMin * magnitudeY;
        }
        else {
          pMin.y = exactPosition[i];
          pMin.x = line->points[0].x + *tMin * magnitudeX;
        }
      }
    }
    else if (p[i] > 0.0) {
      // Inside to Outside
      r = q[i] / p[i];
      if (r < *tMin) {
        return 0;
      }
      else if (r < *tMax) {
        *tMax = r;
        line_index_max = i;
        if (i < 2) {
          pMax.x = exactPosition[i];
          pMax.y = line->points[0].y + *tMax * magnitudeY;
        }
        else {
          pMax.y = exactPosition[i];
          pMax.x = line->points[0].x + *tMax * magnitudeX;
        }
      }
    }
    else if (p[i] == 0.0 && q[i] < 0.0) {
      // Reject
      return 0;
    }
  }

  if (*tMin > *tMax) {
    // Outside
    return 0;
  }

  // Line intersects between tMin and tMax

  if (*tMin > 0.0) {
    line->points[0] = pMin;
  }
  else {
    *tMin = 0.0;
  }

  if (*tMax < 1.0) {
    line->points[1] = pMax;
  }
  else {
    *tMax = 1.0;
  }

  if (*tMin == 0.0 && *tMax == 1.0) {
    // Inside rectangle
    for (unsigned int i = 0; i < 4; i++) {
      // If we are touching an edge, return that edge.
      if (q[i] < 0.00005) {
        if (i < 2) {
          line->points[0].x = exactPosition[i];
        }
        else {
          line->points[0].y = exactPosition[i];
        }
        return i + 1;
      }
    }

    return 0;
  }

  // In a special case when tMin and tMax are 0.0,
  // tMin does not get set and thus does not have
  // a valid line_index_min indicating which edge
  // was the clipping edge... but tMax must be set
  // so return that instead. (This happens when the
  // line clips a corner)
  if (((*tMin) - 0.00005 < (*tMax)) && ((*tMin) + 0.00005 > (*tMax))) {
    return line_index_max + 1;
  }

  return line_index_min + 1;
}

bool Apsis::Geometry::Rectangle::contains(Point* point) const {
  double halfWidth  = width  / 2.0;
  double halfHeight = height / 2.0;

  bool containsPoint = true;
  containsPoint &= point->x >= x - halfWidth;
  containsPoint &= point->y >= y - halfHeight;
  containsPoint &= point->x <  x + halfWidth;
  containsPoint &= point->y <  y + halfHeight;
  return containsPoint;
}

bool Apsis::Geometry::Rectangle::on(Point* point) const {
  double halfWidth  = width  / 2.0;
  double halfHeight = height / 2.0;

  double left   = x - halfWidth;
  double right  = x + halfWidth;
  double top    = y - halfHeight;
  double bottom = y + halfHeight;

  if (point->x == left && point->y >= top && point->y <= bottom) {
    return true;
  }

  if (point->x == right && point->y >= top && point->y <= bottom) {
    return true;
  }

  if (point->y == top && point->x >= left && point->x <= right) {
    return true;
  }

  if (point->y == bottom && point->x >= left && point->x <= right) {
    return true;
  }

  return false;
}

bool Apsis::Geometry::Rectangle::corner(Point* point) const {
  double halfWidth  = width  / 2.0;
  double halfHeight = height / 2.0;

  double left   = x - halfWidth;
  double right  = x + halfWidth;
  double top    = y - halfHeight;
  double bottom = y + halfHeight;

  if (point->x == left && point->y == top) {
    return true;
  }

  if (point->x == left && point->y == bottom) {
    return true;
  }

  if (point->x == right && point->y == top) {
    return true;
  }

  if (point->x == right && point->y == bottom) {
    return true;
  }

  return false;
}

bool Apsis::Geometry::Rectangle::on(Line* line) const {
  if (line->points[0].x == line->points[1].x || line->points[0].y == line->points[1].y) {
    return this->on(&line->points[0]) && this->on(&line->points[1]);
  }

  return false;
}

void Apsis::Geometry::Rectangle::points(Point points[4]) const {
  float halfWidth  = width  / 2.0f;
  float halfHeight = height / 2.0f;

  // Top-left
  points[0].x = x - halfWidth;
  points[0].y = y - halfHeight;

  // Top-right
  points[1].x = x + halfWidth;
  points[1].y = y - halfHeight;

  // Bottom-right
  points[2].x = x + halfWidth;
  points[2].y = y + halfHeight;

  // Bottom-left
  points[3].x = x - halfWidth;
  points[3].y = y + halfHeight;
}

void Apsis::Geometry::Rectangle::lines(Line lines[4]) const {
  Point points[4];
  this->points(points);

  for (int i = 0; i < 4; i++) {
    lines[i].points[0] = points[i];
    lines[i].points[1] = points[(i+1)%4];
  }
}

Apsis::Geometry::Point Apsis::Geometry::Rectangle::center() const {
  Apsis::Geometry::Point p = { this->x, this->y };
  return p;
}

Apsis::Geometry::Line Apsis::Geometry::Rectangle::edge(unsigned int index) const {
  if (index > 3) {
    throw "Rectangle::edge : Index should be between 0 and 3.";
  }

  Apsis::Geometry::Line ret;
  ret.points[0] = this->center();
  ret.points[1] = this->center();

  float half_width  = this->width  / 2.0f;
  float half_height = this->height / 2.0f;

  if (index == 0) { // Left
    ret.points[0].x -= half_width;
    ret.points[1].x -= half_width;
    ret.points[0].y -= half_height;
    ret.points[1].y += half_height;
  }
  else if (index == 1) { // Right
    ret.points[0].x += half_width;
    ret.points[1].x += half_width;
    ret.points[0].y -= half_height;
    ret.points[1].y += half_height;
  }
  else if (index == 2) { // Top
    ret.points[0].x -= half_width;
    ret.points[1].x += half_width;
    ret.points[0].y -= half_height;
    ret.points[1].y -= half_height;
  }
  else if (index == 3) { // Bottom
    ret.points[0].x -= half_width;
    ret.points[1].x += half_width;
    ret.points[0].y += half_height;
    ret.points[1].y += half_height;
  }

  return ret;
}

float Apsis::Geometry::Rectangle::left() const {
  return x - width / 2.0f;
}

float Apsis::Geometry::Rectangle::top() const {
  return y - height / 2.0f;
}

float Apsis::Geometry::Rectangle::right() const {
  return x + width / 2.0f;
}

float Apsis::Geometry::Rectangle::bottom() const {
  return y + height / 2.0f;
}