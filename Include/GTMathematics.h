// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2016
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.3 (2016/08/29)

#pragma once

// Algebra
#include <Mathematics/GteAxisAngle.h>
#include <Mathematics/GteBandedMatrix.h>
#include <Mathematics/GteConvertCoordinates.h>
#include <Mathematics/GteEulerAngles.h>
#include <Mathematics/GteGMatrix.h>
#include <Mathematics/GteGVector.h>
#include <Mathematics/GteMatrix.h>
#include <Mathematics/GteMatrix2x2.h>
#include <Mathematics/GteMatrix3x3.h>
#include <Mathematics/GteMatrix4x4.h>
#include <Mathematics/GtePolynomial1.h>
#include <Mathematics/GteQuaternion.h>
#include <Mathematics/GteRotation.h>
#include <Mathematics/GteVector.h>
#include <Mathematics/GteVector2.h>
#include <Mathematics/GteVector3.h>
#include <Mathematics/GteVector4.h>

// Approximation
#include <Mathematics/GteApprCircle2.h>
#include <Mathematics/GteApprCylinder3.h>
#include <Mathematics/GteApprEllipseByArcs.h>
#include <Mathematics/GteApprEllipse2.h>
#include <Mathematics/GteApprEllipsoid3.h>
#include <Mathematics/GteApprGaussian2.h>
#include <Mathematics/GteApprGaussian3.h>
#include <Mathematics/GteApprGreatCircle3.h>
#include <Mathematics/GteApprHeightLine2.h>
#include <Mathematics/GteApprHeightPlane3.h>
#include <Mathematics/GteApprOrthogonalLine2.h>
#include <Mathematics/GteApprOrthogonalLine3.h>
#include <Mathematics/GteApprOrthogonalPlane3.h>
#include <Mathematics/GteApprParaboloid3.h>
#include <Mathematics/GteApprPolynomial2.h>
#include <Mathematics/GteApprPolynomial3.h>
#include <Mathematics/GteApprPolynomial4.h>
#include <Mathematics/GteApprPolynomialSpecial2.h>
#include <Mathematics/GteApprPolynomialSpecial3.h>
#include <Mathematics/GteApprPolynomialSpecial4.h>
#include <Mathematics/GteApprQuadratic2.h>
#include <Mathematics/GteApprQuadratic3.h>
#include <Mathematics/GteApprQuery.h>
#include <Mathematics/GteApprSphere3.h>

// Arithmetic
#include <Mathematics/GteArithmetic.h>
#include <Mathematics/GteBitHacks.h>
#include <Mathematics/GteBSNumber.h>
#include <Mathematics/GteBSPrecision.h>
#include <Mathematics/GteBSRational.h>
#include <Mathematics/GteIEEEBinary.h>
#include <Mathematics/GteIEEEBinary16.h>
#include <Mathematics/GteUIntegerALU32.h>
#include <Mathematics/GteUIntegerAP32.h>
#include <Mathematics/GteUIntegerFP32.h>

// ComputationalGeometry
#include <Mathematics/GteConstrainedDelaunay2.h>
#include <Mathematics/GteConvexHull2.h>
#include <Mathematics/GteConvexHull3.h>
#include <Mathematics/GteDelaunay2.h>
#include <Mathematics/GteDelaunay2Mesh.h>
#include <Mathematics/GteDelaunay3.h>
#include <Mathematics/GteDelaunay3Mesh.h>
#include <Mathematics/GteEdgeKey.h>
#include <Mathematics/GteETManifoldMesh.h>
#include <Mathematics/GteFeatureKey.h>
#include <Mathematics/GteGenerateMeshUV.h>
#include <Mathematics/GteIsPlanarGraph.h>
#include <Mathematics/GteMinimalCycleBasis.h>
#include <Mathematics/GteMinimumAreaBox2.h>
#include <Mathematics/GteMinimumAreaCircle2.h>
#include <Mathematics/GteMinimumVolumeBox3.h>
#include <Mathematics/GteMinimumVolumeSphere3.h>
#include <Mathematics/GteNearestNeighborQuery.h>
#include <Mathematics/GtePlanarMesh.h>
#include <Mathematics/GtePrimalQuery2.h>
#include <Mathematics/GtePrimalQuery3.h>
#include <Mathematics/GteSeparatePoints2.h>
#include <Mathematics/GteSeparatePoints3.h>
#include <Mathematics/GteTetrahedronKey.h>
#include <Mathematics/GteTriangleKey.h>
#include <Mathematics/GteTriangulateCDT.h>
#include <Mathematics/GteTriangulateEC.h>
#include <Mathematics/GteTSManifoldMesh.h>
#include <Mathematics/GteUniqueVerticesTriangles.h>
#include <Mathematics/GteVEManifoldMesh.h>
#include <Mathematics/GteVETManifoldMesh.h>
#include <Mathematics/GteVertexCollapseMesh.h>

// Containment
#include <Mathematics/GteContCapsule3.h>
#include <Mathematics/GteContCircle2.h>
#include <Mathematics/GteContCylinder3.h>
#include <Mathematics/GteContEllipse2.h>
#include <Mathematics/GteContEllipse2MinCR.h>
#include <Mathematics/GteContEllipsoid3.h>
#include <Mathematics/GteContEllipsoid3MinCR.h>
#include <Mathematics/GteContOrientedBox2.h>
#include <Mathematics/GteContOrientedBox3.h>
#include <Mathematics/GteContPointInPolygon2.h>
#include <Mathematics/GteContPointInPolyhedron3.h>
#include <Mathematics/GteContScribeCircle2.h>
#include <Mathematics/GteContScribeCircle3Sphere3.h>
#include <Mathematics/GteContSphere3.h>

// CurvesSurfacesVolumes
#include <Mathematics/GteBasisFunction.h>
#include <Mathematics/GteBezierCurve.h>
#include <Mathematics/GteBSplineCurve.h>
#include <Mathematics/GteBSplineCurveFit.h>
#include <Mathematics/GteBSplineSurface.h>
#include <Mathematics/GteBSplineSurfaceFit.h>
#include <Mathematics/GteBSplineVolume.h>
#include <Mathematics/GteDarbouxFrame.h>
#include <Mathematics/GteFrenetFrame.h>
#include <Mathematics/GteIndexAttribute.h>
#include <Mathematics/GteMesh.h>
#include <Mathematics/GteNaturalSplineCurve.h>
#include <Mathematics/GteNURBSCurve.h>
#include <Mathematics/GteNURBSSurface.h>
#include <Mathematics/GteNURBSVolume.h>
#include <Mathematics/GteParametricCurve.h>
#include <Mathematics/GteParametricSurface.h>
#include <Mathematics/GteRectangleMesh.h>
#include <Mathematics/GteRectanglePatchMesh.h>
#include <Mathematics/GteRevolutionMesh.h>
#include <Mathematics/GteTCBSplineCurve.h>
#include <Mathematics/GteTubeMesh.h>
#include <Mathematics/GteVertexAttribute.h>

// Distance
#include <Mathematics/GteDCPQuery.h>
#include <Mathematics/GteDistCircle3Circle3.h>
#include <Mathematics/GteDistLine3AlignedBox3.h>
#include <Mathematics/GteDistLine3Circle3.h>
#include <Mathematics/GteDistLine3OrientedBox3.h>
#include <Mathematics/GteDistLine3Rectangle3.h>
#include <Mathematics/GteDistLine3Triangle3.h>
#include <Mathematics/GteDistLineLine.h>
#include <Mathematics/GteDistLineRay.h>
#include <Mathematics/GteDistLineSegment.h>
#include <Mathematics/GteDistPoint3Circle3.h>
#include <Mathematics/GteDistPoint3Cylinder3.h>
#include <Mathematics/GteDistPoint3Frustum3.h>
#include <Mathematics/GteDistPoint3Plane3.h>
#include <Mathematics/GteDistPoint3Rectangle3.h>
#include <Mathematics/GteDistPoint3Tetrahedron3.h>
#include <Mathematics/GteDistPointAlignedBox.h>
#include <Mathematics/GteDistPointHyperellipsoid.h>
#include <Mathematics/GteDistPointLine.h>
#include <Mathematics/GteDistPointOrientedBox.h>
#include <Mathematics/GteDistPointRay.h>
#include <Mathematics/GteDistPointSegment.h>
#include <Mathematics/GteDistPointTriangle.h>
#include <Mathematics/GteDistPointTriangleExact.h>
#include <Mathematics/GteDistRay3AlignedBox3.h>
#include <Mathematics/GteDistRay3OrientedBox3.h>
#include <Mathematics/GteDistRay3Rectangle3.h>
#include <Mathematics/GteDistRay3Triangle3.h>
#include <Mathematics/GteDistRayRay.h>
#include <Mathematics/GteDistRaySegment.h>
#include <Mathematics/GteDistRectangle3Rectangle3.h>
#include <Mathematics/GteDistSegment3AlignedBox3.h>
#include <Mathematics/GteDistSegment3OrientedBox3.h>
#include <Mathematics/GteDistSegment3Rectangle3.h>
#include <Mathematics/GteDistSegment3Triangle3.h>
#include <Mathematics/GteDistSegmentSegment.h>
#include <Mathematics/GteDistSegmentSegmentExact.h>
#include <Mathematics/GteDistTriangle3Rectangle3.h>
#include <Mathematics/GteDistTriangle3Triangle3.h>

// Functions
#include <Mathematics/GteACosEstimate.h>
#include <Mathematics/GteASinEstimate.h>
#include <Mathematics/GteATanEstimate.h>
#include <Mathematics/GteChebyshevRatio.h>
#include <Mathematics/GteConstants.h>
#include <Mathematics/GteCosEstimate.h>
#include <Mathematics/GteExp2Estimate.h>
#include <Mathematics/GteExpEstimate.h>
#include <Mathematics/GteFunctions.h>
#include <Mathematics/GteInvSqrtEstimate.h>
#include <Mathematics/GteLog2Estimate.h>
#include <Mathematics/GteLogEstimate.h>
#include <Mathematics/GteSinEstimate.h>
#include <Mathematics/GteSlerpEstimate.h>
#include <Mathematics/GteSqrtEstimate.h>
#include <Mathematics/GteTanEstimate.h>

// GeometricPrimitives
#include <Mathematics/GteAlignedBox.h>
#include <Mathematics/GteArc2.h>
#include <Mathematics/GteCapsule.h>
#include <Mathematics/GteCircle3.h>
#include <Mathematics/GteCone.h>
#include <Mathematics/GteCylinder3.h>
#include <Mathematics/GteEllipse3.h>
#include <Mathematics/GteFrustum3.h>
#include <Mathematics/GteHalfspace.h>
#include <Mathematics/GteHyperellipsoid.h>
#include <Mathematics/GteHyperplane.h>
#include <Mathematics/GteHypersphere.h>
#include <Mathematics/GteLine.h>
#include <Mathematics/GteOrientedBox.h>
#include <Mathematics/GtePolygon2.h>
#include <Mathematics/GtePolyhedron3.h>
#include <Mathematics/GteRay.h>
#include <Mathematics/GteRectangle.h>
#include <Mathematics/GteSector2.h>
#include <Mathematics/GteSegment.h>
#include <Mathematics/GteTetrahedron3.h>
#include <Mathematics/GteTorus3.h>
#include <Mathematics/GteTriangle.h>

// Interpolation
#include <Mathematics/GteIntpAkima1.h>
#include <Mathematics/GteIntpAkimaNonuniform1.h>
#include <Mathematics/GteIntpAkimaUniform1.h>
#include <Mathematics/GteIntpAkimaUniform2.h>
#include <Mathematics/GteIntpAkimaUniform3.h>
#include <Mathematics/GteIntpBicubic2.h>
#include <Mathematics/GteIntpBilinear2.h>
#include <Mathematics/GteIntpLinearNonuniform2.h>
#include <Mathematics/GteIntpLinearNonuniform3.h>
#include <Mathematics/GteIntpQuadraticNonuniform2.h>
#include <Mathematics/GteIntpSphere2.h>
#include <Mathematics/GteIntpThinPlateSpline2.h>
#include <Mathematics/GteIntpThinPlateSpline3.h>
#include <Mathematics/GteIntpTricubic3.h>
#include <Mathematics/GteIntpTrilinear3.h>
#include <Mathematics/GteIntpVectorField2.h>

// Intersection
#include <Mathematics/GteFIQuery.h>
#include <Mathematics/GteIntrAlignedBox2AlignedBox2.h>
#include <Mathematics/GteIntrAlignedBox2OrientedBox2.h>
#include <Mathematics/GteIntrAlignedBox3AlignedBox3.h>
#include <Mathematics/GteIntrAlignedBox3Cone3.h>
#include <Mathematics/GteIntrAlignedBox3OrientedBox3.h>
#include <Mathematics/GteIntrArc2Arc2.h>
#include <Mathematics/GteIntrCapsule3Capsule3.h>
#include <Mathematics/GteIntrCircle2Arc2.h>
#include <Mathematics/GteIntrCircle2Circle2.h>
#include <Mathematics/GteIntrConvexPolygonPlane.h>
#include <Mathematics/GteIntrDisk2Sector2.h>
#include <Mathematics/GteIntrEllipse2Ellipse2.h>
#include <Mathematics/GteIntrEllipsoid3Ellipsoid3.h>
#include <Mathematics/GteIntrHalfspace2Polygon2.h>
#include <Mathematics/GteIntrHalfspace3Capsule3.h>
#include <Mathematics/GteIntrHalfspace3Cylinder3.h>
#include <Mathematics/GteIntrHalfspace3Ellipsoid3.h>
#include <Mathematics/GteIntrHalfspace3OrientedBox3.h>
#include <Mathematics/GteIntrHalfspace3Segment3.h>
#include <Mathematics/GteIntrHalfspace3Sphere3.h>
#include <Mathematics/GteIntrHalfspace3Triangle3.h>
#include <Mathematics/GteIntrIntervals.h>
#include <Mathematics/GteIntrLine2AlignedBox2.h>
#include <Mathematics/GteIntrLine2Arc2.h>
#include <Mathematics/GteIntrLine2Circle2.h>
#include <Mathematics/GteIntrLine2Line2.h>
#include <Mathematics/GteIntrLine2OrientedBox2.h>
#include <Mathematics/GteIntrLine2Ray2.h>
#include <Mathematics/GteIntrLine2Segment2.h>
#include <Mathematics/GteIntrLine2Triangle2.h>
#include <Mathematics/GteIntrLine3AlignedBox3.h>
#include <Mathematics/GteIntrLine3Capsule3.h>
#include <Mathematics/GteIntrLine3Cone3.h>
#include <Mathematics/GteIntrLine3Cylinder3.h>
#include <Mathematics/GteIntrLine3Ellipsoid3.h>
#include <Mathematics/GteIntrLine3OrientedBox3.h>
#include <Mathematics/GteIntrLine3Plane3.h>
#include <Mathematics/GteIntrLine3Sphere3.h>
#include <Mathematics/GteIntrLine3Triangle3.h>
#include <Mathematics/GteIntrOrientedBox2Circle2.h>
#include <Mathematics/GteIntrOrientedBox2Cone2.h>
#include <Mathematics/GteIntrOrientedBox2OrientedBox2.h>
#include <Mathematics/GteIntrOrientedBox2Sector2.h>
#include <Mathematics/GteIntrOrientedBox3Cone3.h>
#include <Mathematics/GteIntrOrientedBox3Frustum3.h>
#include <Mathematics/GteIntrOrientedBox3OrientedBox3.h>
#include <Mathematics/GteIntrOrientedBox3Sphere3.h>
#include <Mathematics/GteIntrPlane3Capsule3.h>
#include <Mathematics/GteIntrPlane3Circle3.h>
#include <Mathematics/GteIntrPlane3Cylinder3.h>
#include <Mathematics/GteIntrPlane3Ellipsoid3.h>
#include <Mathematics/GteIntrPlane3OrientedBox3.h>
#include <Mathematics/GteIntrPlane3Plane3.h>
#include <Mathematics/GteIntrPlane3Sphere3.h>
#include <Mathematics/GteIntrPlane3Triangle3.h>
#include <Mathematics/GteIntrRay2AlignedBox2.h>
#include <Mathematics/GteIntrRay2Arc2.h>
#include <Mathematics/GteIntrRay2Circle2.h>
#include <Mathematics/GteIntrRay2OrientedBox2.h>
#include <Mathematics/GteIntrRay2Ray2.h>
#include <Mathematics/GteIntrRay2Segment2.h>
#include <Mathematics/GteIntrRay2Triangle2.h>
#include <Mathematics/GteIntrRay3AlignedBox3.h>
#include <Mathematics/GteIntrRay3Capsule3.h>
#include <Mathematics/GteIntrRay3Cone3.h>
#include <Mathematics/GteIntrRay3Cylinder3.h>
#include <Mathematics/GteIntrRay3Ellipsoid3.h>
#include <Mathematics/GteIntrRay3OrientedBox3.h>
#include <Mathematics/GteIntrRay3Plane3.h>
#include <Mathematics/GteIntrRay3Sphere3.h>
#include <Mathematics/GteIntrRay3Triangle3.h>
#include <Mathematics/GteIntrSegment2AlignedBox2.h>
#include <Mathematics/GteIntrSegment2Arc2.h>
#include <Mathematics/GteIntrSegment2Circle2.h>
#include <Mathematics/GteIntrSegment2OrientedBox2.h>
#include <Mathematics/GteIntrSegment2Segment2.h>
#include <Mathematics/GteIntrSegment2Triangle2.h>
#include <Mathematics/GteIntrSegment3AlignedBox3.h>
#include <Mathematics/GteIntrSegment3Capsule3.h>
#include <Mathematics/GteIntrSegment3Cone3.h>
#include <Mathematics/GteIntrSegment3Cylinder3.h>
#include <Mathematics/GteIntrSegment3Ellipsoid3.h>
#include <Mathematics/GteIntrSegment3OrientedBox3.h>
#include <Mathematics/GteIntrSegment3Plane3.h>
#include <Mathematics/GteIntrSegment3Sphere3.h>
#include <Mathematics/GteIntrSegment3Triangle3.h>
#include <Mathematics/GteIntrSphere3Cone3.h>
#include <Mathematics/GteIntrSphere3Frustum3.h>
#include <Mathematics/GteIntrSphere3Sphere3.h>
#include <Mathematics/GteIntrTriangle3OrientedBox3.h>
#include <Mathematics/GteTIQuery.h>

// NumericalMethods
#include <Mathematics/GteCubicRootsQR.h>
#include <Mathematics/GteGaussianElimination.h>
#include <Mathematics/GteIntegration.h>
#include <Mathematics/GteLinearSystem.h>
#include <Mathematics/GteMinimize1.h>
#include <Mathematics/GteMinimizeN.h>
#include <Mathematics/GteOdeEuler.h>
#include <Mathematics/GteOdeImplicitEuler.h>
#include <Mathematics/GteOdeMidpoint.h>
#include <Mathematics/GteOdeRungeKutta4.h>
#include <Mathematics/GteOdeSolver.h>
#include <Mathematics/GteQuarticRootsQR.h>
#include <Mathematics/GteRootsBisection.h>
#include <Mathematics/GteRootsBrentsMethod.h>
#include <Mathematics/GteRootsPolynomial.h>
#include <Mathematics/GteSingularValueDecomposition.h>
#include <Mathematics/GteSymmetricEigensolver.h>
#include <Mathematics/GteSymmetricEigensolver2x2.h>
#include <Mathematics/GteSymmetricEigensolver3x3.h>
#include <Mathematics/GteUnsymmetricEigenvalues.h>

// Projection
#include <Mathematics/GteProjection.h>

// SIMD
#if defined(__MSWINDOWS__)
#include <Mathematics/MSW/GteIntelSSE.h>
#endif

