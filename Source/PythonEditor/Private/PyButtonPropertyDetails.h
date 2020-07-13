//////////////////////////////////////////////////////////////////////////
// DK Begin: ID(#DK_PyPropertyButton) modifier:(xingtongli)
#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

class IPropertyHandle;

class FPyButtonPropertyDetails : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShareable(new FPyButtonPropertyDetails());
	}

	// IDetailCustomization interface
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

	FReply OnClicked();

	void RunFile(const FString& FilePath, const FString& FunctionName, UObject* ParamObj);

	TWeakObjectPtr<UObject> SelectObject;
	TSharedPtr<IPropertyHandle> ScriptPathHandle;
	TSharedPtr<IPropertyHandle> FunctionHandle;
};
// DK End
//////////////////////////////////////////////////////////////////////////
