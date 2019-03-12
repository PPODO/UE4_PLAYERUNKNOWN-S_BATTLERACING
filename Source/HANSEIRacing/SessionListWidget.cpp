#include "SessionListWidget.h"

void USessionListWidget::SetSessionInfomation(const FString & Name, const int32 & Current, const int32 & Max, const bool Use) {
	m_SessionName = Name;
	m_CurrentPlayer = Current;
	m_MaxPlayer = Max;
	m_UsePassword = Use;
}

void USessionListWidget::SetParentWidgetClass(UUserWidget* Widget) {
	m_ParentWidget = Widget;
}