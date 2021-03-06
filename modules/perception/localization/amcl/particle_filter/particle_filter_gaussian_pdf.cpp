/****************************************************************************
 *  Copyright (C) 2018 RoboMaster.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

/*
 *  Player - One Hell of a Robot Server
 *  Copyright (C) 2000  Brian Gerkey   &  Kasper Stoy
 *                      gerkey@usc.edu    kaspers@robotics.usc.edu
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "modules/perception/localization/amcl/particle_filter/particle_filter_gaussian_pdf.h"
namespace rrts {
namespace perception {
namespace localization {

ParticleFilterGaussianPdf::ParticleFilterGaussianPdf(const math::Vec3d &mean,
                                                     const math::Mat3d &covariance) {
  this->mean_ = mean;
  this->covariance_ = covariance;
  math::EigenDecomposition(this->covariance_, &this->covariance_rotation_, &this->covariance_diagonal_);

}

math::Vec3d ParticleFilterGaussianPdf::GenerateSample() {
  int i = 0;
  math::Vec3d random_vec;
  math::Vec3d mean_vec;

  for (i = 0; i < this->covariance_diagonal_.size(); i++) {
    double sigma = this->covariance_diagonal_(i);
    random_vec(i) = RandomGaussianNum(sigma);
  }

  for (i = 0; i < this->mean_.size(); i++) {
    mean_vec(i) = this->mean_(i);
    for (int j = 0; j < this->mean_.size(); ++j) {
      mean_vec(i) += this->covariance_rotation_(i, j) * random_vec(j);
    }
  }

  return mean_vec;
}

// Draw randomly from a zero-mean Gaussian distribution, with standard
// deviation sigma.
// We use the polar form of the Box-Muller transformation, explained here:
//   http://www.taygeta.com/random/gaussian.html
double ParticleFilterGaussianPdf::RandomGaussianNum(double sigma) {
  double x1, x2, w, r;
  do {
    do { r = drand48(); } while (r == 0.0);
    x1 = 2.0 * r - 1.0;
    do { r = drand48(); } while (r == 0.0);
    x2 = 2.0 * r - 1.0;
    w = x1 * x1 + x2 * x2;
  } while (w > 1.0 || w == 0.0);

  return (sigma * x2 * std::sqrt(-2.0 * std::log(w) / w));
}

}
}
}
