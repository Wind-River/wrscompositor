#ifndef QDUDUREGIPLATFORMINPUTCONTEXT_H
#define QDUDUREGIPLATFORMINPUTCONTEXT_H

#include <qpa/qplatforminputcontext.h>

QT_BEGIN_NAMESPACE

class QDBusInterface;

class QDuduregiPlatformInputContext : public QPlatformInputContext
{
    Q_OBJECT

public:
    QDuduregiPlatformInputContext();
    ~QDuduregiPlatformInputContext();

    bool isValid() const Q_DECL_OVERRIDE;
    void update(Qt::InputMethodQueries) Q_DECL_OVERRIDE;
    void setFocusObject(QObject *object) Q_DECL_OVERRIDE;

    void showInputPanel() Q_DECL_OVERRIDE;
    void hideInputPanel() Q_DECL_OVERRIDE;
    bool isInputPanelVisible() const Q_DECL_OVERRIDE;

private slots:
    void keyboardSpecialKeyPressed(const QString &keycode);
    void keyboardKeyPressed(const QString &keycode);

private:
    QDBusInterface *m_keyboardInterface;
    QObject *m_focusObject;
};

QT_END_NAMESPACE

#endif
