#include <algorithm>
#include <limits>
#include <ostream>
#include <Types.h>

template <UInt D, typename TCoordinateType = double>
class DPosition
{
	public:

		/// Coordinate type
		typedef TCoordinateType CoordinateType;
		/// Mutable iterator
		typedef CoordinateType* Iterator;
		/// Non-mutable iterator
		typedef const CoordinateType* ConstIterator;
		/// Dimensions
		enum
		{
			DIMENSION = D
		};
		/**
		  @name STL compatibility type definitions
		 */
		//@{
		typedef CoordinateType value_type;
		typedef CoordinateType& reference;
		typedef CoordinateType* pointer;
		typedef CoordinateType* iterator;
		typedef const CoordinateType* const_iterator;
		//@}

		/**
		  @name Constructors and Destructor
		 */
		//@{
		/**
		  @brief Default constructor.

		  Creates a position with all coordinates zero.
		 */
		DPosition()
		{
			clear();
		}

		/// Destructor (not-virtual as this will save a lot of space!)
		~DPosition()
		{
		}

		/// Constructor that fills all dimensions with the value @p x
		DPosition(CoordinateType x)
		{
			std::fill(&(coordinate_[0]), &(coordinate_[D]), x);
		}

		/// Copy constructor
		DPosition(const DPosition& pos)
		{
			std::copy(&(pos.coordinate_[0]), &(pos.coordinate_[D]),
					&(coordinate_[0]));
		}

		/// Move constructor
		DPosition(DPosition&& rhs) noexcept
		{
			// NOTE: do not change this before testing with nightly Windows builds ( = default causes segfault)
			std::move(std::begin(rhs.coordinate_), std::end(rhs.coordinate_), &coordinate_[0]);
		}

		/// Constructor only for DPosition<2> that takes two Coordinates.
		DPosition(CoordinateType x, CoordinateType y)
		{
			coordinate_[0] = x;
			coordinate_[1] = y;
		}

		/// Assignment operator
		DPosition& operator=(const DPosition& source)
		{
			if (&source == this) return *this;

			std::copy(&(source.coordinate_[0]), &(source.coordinate_[D]),
					&(coordinate_[0]));

			return *this;
		}

		//@}

		/**@name Accessors */
		//@{

		///Const accessor for the dimensions
		CoordinateType operator[](Size index) const
		{
			return coordinate_[index];
		}

		///Accessor for the dimensions
		CoordinateType& operator[](Size index)
		{
			return coordinate_[index];
		}

		///Name accessor for the first dimension. Only for DPosition<2>, for visualization.
		CoordinateType getX() const
		{
			return coordinate_[0];
		}

		///Name accessor for the second dimension. Only for DPosition<2>, for visualization.
		CoordinateType getY() const
		{
			return coordinate_[1];
		}

		///Name mutator for the first dimension. Only for DPosition<2>, for visualization.
		void setX(CoordinateType c)
		{
			coordinate_[0] = c;
		}

		///Name mutator for the second dimension. Only for DPosition<2>, for visualization.
		void setY(CoordinateType c)
		{
			coordinate_[1] = c;
		}

		/// Equality operator
		bool operator==(const DPosition& point) const
		{
			for (Size i = 0; i < D; i++)
			{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wfloat-equal"
				if (coordinate_[i] != point.coordinate_[i]) return false;

#pragma clang diagnostic pop
			}
			return true;
		}

		/// Equality operator
		bool operator!=(const DPosition& point) const
		{
			return !(operator==(point));
		}

		/**
		  @brief Lexicographical less than operator.
		  Lexicographical comparison from dimension 0 to dimension D-1 is done.
		 */
		bool operator<(const DPosition& point) const
		{
			for (Size i = 0; i < D; i++)
			{
				if (coordinate_[i] < point.coordinate_[i]) return true;

				if (coordinate_[i] > point.coordinate_[i]) return false;
			}
			return false;
		}

		/// Lexicographical greater less or equal operator.
		bool operator<=(const DPosition& point) const
		{
			for (Size i = 0; i < D; i++)
			{
				if (coordinate_[i] < point.coordinate_[i]) return true;

				if (coordinate_[i] > point.coordinate_[i]) return false;
			}
			return true;
		}

		/// Spatially (geometrically) less or equal operator. All coordinates must be "<=".
		bool spatiallyLessEqual(const DPosition& point) const
		{
			for (Size i = 0; i < D; i++)
			{
				if (coordinate_[i] > point.coordinate_[i]) return false;
			}
			return true;
		}

		/// Spatially (geometrically) greater or equal operator. All coordinates must be ">=".
		bool spatiallyGreaterEqual(const DPosition& point) const
		{
			for (Size i = 0; i < D; i++)
			{
				if (coordinate_[i] < point.coordinate_[i]) return false;
			}
			return true;
		}

		/// Lexicographical greater than operator.
		bool operator>(const DPosition& point) const
		{
			return !(operator<=(point));
		}

		/// Lexicographical greater or equal operator.
		bool operator>=(const DPosition& point) const
		{
			return !operator<(point);
		}

		/// Addition (a bit inefficient)
		DPosition operator+(const DPosition& point) const
		{
			DPosition result(*this);
			for (Size i = 0; i < D; ++i)
			{
				result.coordinate_[i] += point.coordinate_[i];
			}
			return result;
		}

		/// Addition
		DPosition& operator+=(const DPosition& point)
		{
			for (Size i = 0; i < D; ++i)
			{
				coordinate_[i] += point.coordinate_[i];
			}
			return *this;
		}

		/// Subtraction (a bit inefficient)
		DPosition operator-(const DPosition& point) const
		{
			DPosition result(*this);
			for (Size i = 0; i < D; ++i)
			{
				result.coordinate_[i] -= point.coordinate_[i];
			}
			return result;
		}

		/// Subtraction
		DPosition& operator-=(const DPosition& point)
		{
			for (Size i = 0; i < D; ++i)
			{
				coordinate_[i] -= point.coordinate_[i];
			}
			return *this;
		}

		/// Negation (a bit inefficient)
		DPosition   operator-() const
		{
			DPosition<D, CoordinateType> result(*this);
			for (Size i = 0; i < D; ++i)
			{
				result.coordinate_[i] = -result.coordinate_[i];
			}
			return result;
		}

		/// Inner product
		CoordinateType operator*(const DPosition& point) const
		{
			CoordinateType prod(0);
			for (Size i = 0; i < D; ++i)
			{
				prod += (point.coordinate_[i] * coordinate_[i]);
			}
			return prod;
		}

		/// Scalar multiplication
		DPosition& operator*=(CoordinateType scalar)
		{
			for (Size i = 0; i < D; ++i)
			{
				coordinate_[i] *= scalar;
			}
			return *this;
		}

		/// Scalar division
		DPosition& operator/=(CoordinateType scalar)
		{
			for (Size i = 0; i < D; ++i)
			{
				coordinate_[i] /= scalar;
			}
			return *this;
		}

		/// Returns the number of dimensions
		static Size size()
		{
			return D;
		}

		/// Set all dimensions to zero
		void clear()
		{
			for (Size i = 0; i < D; ++i)
			{
				coordinate_[i] = static_cast<CoordinateType>(0);
			}
		}

		//@}

		/** @name Static values */
		//@{
		/// all zero
		inline static const DPosition zero()
		{
			return DPosition(0);
		}

		/// smallest positive
		inline static const DPosition minPositive()
		{
			return DPosition((std::numeric_limits<typename DPosition::CoordinateType>::min)());
		}

		/// smallest negative
		inline static const DPosition minNegative()
		{
			return DPosition(-(std::numeric_limits<typename DPosition::CoordinateType>::max)());
		}

		/// largest positive
		inline static const DPosition maxPositive()
		{
			return DPosition((std::numeric_limits<typename DPosition::CoordinateType>::max)());
		}

		//@}

		/** @name Iteration */
		//@{
		/// Non-mutable begin iterator
		ConstIterator begin() const
		{
			return &(coordinate_[0]);
		}

		/// Non-mutable end iterator
		ConstIterator end() const
		{
			return &(coordinate_[0]) + D;
		}

		/// Mutable begin iterator
		Iterator begin()
		{
			return &(coordinate_[0]);
		}

		/// Mutable end iterator
		Iterator end()
		{
			return &(coordinate_[0]) + D;
		}

		//@}

	protected:
		CoordinateType coordinate_[D];

}; // DPosition

/// Scalar multiplication (a bit inefficient)
	template <UInt D, typename TCoordinateType>
DPosition<D, TCoordinateType> operator*(DPosition<D, TCoordinateType> position, typename DPosition<D, TCoordinateType>::CoordinateType scalar)
{
	for (Size i = 0; i < D; ++i)
	{
		position[i] *= scalar;
	}
	return position;
}

/// Scalar multiplication (a bit inefficient)
	template <UInt D, typename TCoordinateType>
DPosition<D, TCoordinateType> operator*(typename DPosition<D, TCoordinateType>::CoordinateType scalar, DPosition<D, TCoordinateType> position)
{
	for (Size i = 0; i < D; ++i)
	{
		position[i] *= scalar;
	}
	return position;
}

/// Scalar multiplication (a bit inefficient)
	template <UInt D, typename TCoordinateType>
DPosition<D, TCoordinateType> operator/(DPosition<D, TCoordinateType> position, typename DPosition<D, TCoordinateType>::CoordinateType scalar)
{
	for (Size i = 0; i < D; ++i)
	{
		position[i] /= scalar;
	}
	return position;
}

/// Print the contents to a stream.
	template <UInt D, typename TCoordinateType>
std::ostream& operator<<(std::ostream& os, const DPosition<D, TCoordinateType>& pos)
{
	os << precisionWrapper(pos[0]);
	for (UInt i = 1; i < D; ++i)
	{
		os << ' ' << precisionWrapper(pos[i]);
	}
	return os;
}

