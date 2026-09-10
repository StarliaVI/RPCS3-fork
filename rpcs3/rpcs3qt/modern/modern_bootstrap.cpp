#include "modern_shell.h"

#include <QAction>
#include <QApplication>
#include <QMenu>
#include <QTimer>

// This symbol is intentionally referenced by rpcs3/main.cpp.
// rpcs3_ui is a static library, so without an explicit reference the linker
// is free to discard this translation unit before Qt's startup hook can run.
void force_link_modern_ui_bootstrap()
{
}

namespace
{
	constexpr int max_install_attempts = 80;
	constexpr int retry_delay_ms = 100;

	bool attach_preview_action()
	{
		for (QWidget* widget : QApplication::topLevelWidgets())
		{
			if (!widget || widget->objectName() != QStringLiteral("main_window"))
			{
				continue;
			}

			QMenu* view_menu = widget->findChild<QMenu*>(QStringLiteral("menuView"));
			if (!view_menu)
			{
				continue;
			}

			if (view_menu->findChild<QAction*>(QStringLiteral("modernUiPreviewAct")))
			{
				return true;
			}

			auto* preview_action = new QAction(QObject::tr("Modern UI Preview"), view_menu);
			preview_action->setObjectName(QStringLiteral("modernUiPreviewAct"));
			view_menu->insertAction(view_menu->actions().isEmpty() ? nullptr : view_menu->actions().constFirst(), preview_action);
			view_menu->insertSeparator(view_menu->actions().value(1, nullptr));

			QObject::connect(preview_action, &QAction::triggered, widget, []
			{
				auto* preview = new modern_shell();
				preview->setAttribute(Qt::WA_DeleteOnClose);
				preview->setWindowFlag(Qt::Window, true);
				preview->setWindowTitle(QObject::tr("RPCS3 Modern UI Preview"));
				preview->resize(1440, 860);
				preview->setMinimumSize(960, 640);
				preview->show();
			});

			return true;
		}

		return false;
	}

	void try_attach_preview_action(int attempt)
	{
		if (attach_preview_action() || attempt >= max_install_attempts)
		{
			return;
		}

		QTimer::singleShot(retry_delay_ms, [attempt]
		{
			try_attach_preview_action(attempt + 1);
		});
	}

	void install_modern_ui_bootstrap()
	{
		QTimer::singleShot(0, []
		{
			try_attach_preview_action(0);
		});
	}
}

Q_COREAPP_STARTUP_FUNCTION(install_modern_ui_bootstrap)
