#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "drawing/GeometricTypes.hpp"
#include "drawing/Color.hpp"
#include "drawing/PointTransform.hpp"
#include "drawing/ICanvas.hpp"
#include "drawing/Figure.hpp"
#include "drawing/Range.hpp"
#include <string>
#include "HelperFunctions.hpp"

using namespace funcdraw::drawing;
using namespace std::string_literals;

using ::testing::_;
using ::testing::Eq;
using ::testing::Truly;

namespace test
{
	class MockCanvas : public ICanvas
	{
	public:
		MOCK_METHOD3(drawLines,
			void(const MultiLine& lines,
				std::vector<Color> colors,
				const PointTransform& figureTransform));
	};

	class TransformEq
	{
		boost::numeric::ublas::matrix<double> matrix;
	public:
		explicit TransformEq(const PointTransform& transformation)
			: matrix(transformation.getTransformation().matrix()) {}

		bool operator()(const PointTransform& other) const
		{
			auto& otherMatrix = other.getTransformation().matrix();

			if (matrix.size1() != otherMatrix.size1()
				|| matrix.size2() != otherMatrix.size2())
				return false;

			for (auto i = 0; i < matrix.size1(); ++i)
				for (auto j = 0; j < matrix.size2(); ++j)
					if (std::abs(matrix(i, j) - otherMatrix(i, j)) > std::numeric_limits<double>::epsilon())
						return false;
			return true;
		}
	};

	class MultiLineEq
	{
		MultiLine multiLine;

	public:
		explicit MultiLineEq(const MultiLine& multiLine): multiLine(multiLine) {}

		bool operator()(const MultiLine& other) const 
		{
			if (multiLine.size() != other.size())
				return false;

			for(auto i = 0; i < multiLine.size(); ++i)
			{
				if (static_cast<const std::vector<Point>&>(multiLine[i]) != static_cast<const std::vector<Point>&>(other[i]))
					return false;
			}
			return true;
		}
	};

	TEST(CanvasTest, DrawLineTest)
	{
		MockCanvas canvas;
		Figure figure;

		figure.addFunction("3*x"s, Color::PURPLE);
		figure.addFunction([](double x) {return 100; }, Color::CYAN);
		figure.addFunction([](double x) {return -x; }, Color::GREEN);
		
		EXPECT_CALL(canvas, 
			drawLines(
				Truly(MultiLineEq({
						{ { -1., -3. }, { 0., 0. }, { 1., 3. }, { 2., 6. }},
						{ { -1., 100 }, { 0., 100 }, { 1., 100 }, { 2., 100 } },
						{ { -1., 1 }, { 0., 0 }, { 1., -1 }, { 2., -2 } }
						})), 
				std::vector<Color>{ Color::PURPLE, Color::CYAN, Color::GREEN },
				Truly(TransformEq(PointTransform(-1, 2, -3, 100)))))
			.Times(1);

		figure.draw(canvas, Range(-1, 2, 4));
	}
}
