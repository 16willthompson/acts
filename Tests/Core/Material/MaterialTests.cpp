// This file is part of the Acts project.
//
// Copyright (C) 2017-2018 CERN for the benefit of the Acts project
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// clang-format off
#define BOOST_TEST_MODULE Material Tests
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
// clang-format on

#include <limits>

#include "Acts/Material/Material.hpp"
#include "Acts/Tests/CommonHelpers/FloatComparisons.hpp"
#include "Acts/Utilities/Units.hpp"

using namespace Acts::UnitLiterals;

static constexpr auto eps = 2 * std::numeric_limits<float>::epsilon();

BOOST_AUTO_TEST_SUITE(material)

BOOST_AUTO_TEST_CASE(construct_vacuum) {
  // default constructor build invalid material a.k.a. vacuum
  Acts::Material vacuum;
  BOOST_TEST(!vacuum);
}

BOOST_AUTO_TEST_CASE(construct_something) {
  // anything with non-zero A is valid material
  Acts::Material notVacuum(1, 2, 3, 4, 5);
  BOOST_TEST(!!notVacuum);
}

// example values for silicon
static constexpr float SiX0 = 9.370_cm;
static constexpr float SiL0 = 46.52_cm;
static constexpr float SiAr = 28.0855;
static constexpr float SiZ = 14.0;
static constexpr float SiRho = 2.329_g / 1_cm3;
// manually calculated
static constexpr float SiNe = 1.160954941 / 1_cm3;
static constexpr float SiI = 172.042290036_eV;

BOOST_AUTO_TEST_CASE(units) {
  Acts::Material silicon(SiX0, SiL0, SiAr, SiZ, SiRho);

  // check values directly
  CHECK_CLOSE_REL(silicon.X0(), SiX0, eps);
  CHECK_CLOSE_REL(silicon.L0(), SiL0, eps);
  CHECK_CLOSE_REL(silicon.Ar(), SiAr, eps);
  CHECK_CLOSE_REL(silicon.Z(), SiZ, eps);
  CHECK_CLOSE_REL(silicon.rho(), SiRho, eps);
  // check values w/ different units if possible
  CHECK_CLOSE_REL(silicon.X0(), 93.70_mm, eps);
  CHECK_CLOSE_REL(silicon.X0(), 0.09370_m, eps);
  CHECK_CLOSE_REL(silicon.L0(), 465.2_mm, eps);
  CHECK_CLOSE_REL(silicon.L0(), 0.4652_m, eps);
  CHECK_CLOSE_REL(silicon.rho(), 0.002329_kg / 1_cm3, eps);
  CHECK_CLOSE_REL(silicon.rho(), 0.002329_g / 1_mm3, eps);
  // check derived values
  CHECK_CLOSE_REL(silicon.molarElectronDensity(), SiNe, eps);
  CHECK_CLOSE_REL(silicon.meanExcitationEnergy(), SiI, eps);
}

// encode values as classification vector
static Acts::ActsVectorF<5> makeSiClassificationNumbers() {
  Acts::ActsVectorF<5> values;
  values[Acts::Material::eX0] = SiX0;
  values[Acts::Material::eL0] = SiL0;
  values[Acts::Material::eAr] = SiAr;
  values[Acts::Material::eZ] = SiZ;
  values[Acts::Material::eRho] = SiRho;
  return values;
}

BOOST_AUTO_TEST_CASE(classification_numbers) {
  const auto numbers = makeSiClassificationNumbers();
  Acts::Material fromNumbers(numbers);
  Acts::Material manual(SiX0, SiL0, SiAr, SiZ, SiRho);

  BOOST_TEST(fromNumbers == manual);
  CHECK_CLOSE_REL(fromNumbers.classificationNumbers(), numbers, eps);
  CHECK_CLOSE_REL(manual.classificationNumbers(), numbers, eps);
}

BOOST_AUTO_TEST_SUITE_END()
