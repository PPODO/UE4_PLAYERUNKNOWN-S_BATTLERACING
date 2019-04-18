#include "MainMenuWidget.h"
#include "SocketComponent.h"
#include "SessionList.h"

void UMainMenuWidget::NativeConstruct() {
	Super::NativeConstruct();
	
	m_bIsPopup = false;
	m_bIsRefresing = false;
	m_CurrentPage = 0;
}

TArray<UUserWidget*> UMainMenuWidget::CreateSessionList() {
	TArray<UUserWidget*> WidgetClass;

	if (IsInGameThread() && m_WidgetClass) {
		for (auto It : m_SessionList.m_Sessions) {
			auto NewWidget = CreateWidget<USessionList>(GetWorld(), m_WidgetClass);
			if (NewWidget) {
				NewWidget->m_ParentWidget = this;
				NewWidget->AddToViewport();
				NewWidget->SetSessionInformation(ANSI_TO_TCHAR(It.m_SesisonName.c_str()), It.m_CurrentPlayer, It.m_MaxPlayer, It.m_bUsePassword);
				WidgetClass.Add(NewWidget);
			}
		}
		m_SessionList.m_Sessions.clear();
		m_bIsRefresing = false;
	}
	return WidgetClass;
}