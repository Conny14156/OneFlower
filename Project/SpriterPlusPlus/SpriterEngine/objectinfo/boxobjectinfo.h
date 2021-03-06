#ifndef BOXOBJECTINFO_H
#define BOXOBJECTINFO_H

#include "universalobjectinterface.h"

#include "angleinfo.h"

namespace SpriterEngine
{
	class BoxObjectInfo : public UniversalObjectInterface
	{
	public:
		BoxObjectInfo();

		point getPosition() const override;
		real getAngle() const override;
		point getScale() const override;
		real getAlpha() const override;
		point getPivot() const override;

		void setPosition(const point &newPosition) override;
		void setAngle(real newAngle) override;
		void setScale(const point &newScale) override;
		void setAlpha(real newAlpha) override;
		void setPivot(const point &newPivot) override;
		void setSpin(s_int newSpin) override;

		void setObjectToLinear(UniversalObjectInterface *bObject, real t, UniversalObjectInterface *resultObject) override;

	private:
		point position;
		AngleInfo angle;
		point scale;
		point pivot;
		real alpha;
	};
}

#endif // BOXOBJECTINFO_H
