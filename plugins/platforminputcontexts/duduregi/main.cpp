#include <qpa/qplatforminputcontextplugin_p.h>
#include <QtCore/QStringList>
#include "qduduregiplatforminputcontext.h"

QT_BEGIN_NAMESPACE

class QDuduregiPlatformInputContextPlugin : public QPlatformInputContextPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QPlatformInputContextFactoryInterface_iid FILE "duduregi.json")

public:
    QDuduregiPlatformInputContext *create(const QString&, const QStringList&) Q_DECL_OVERRIDE;
};

QDuduregiPlatformInputContext *QDuduregiPlatformInputContextPlugin::create(const QString& system, const QStringList& paramList)
{
    Q_UNUSED(paramList);

    if (system.compare(system, QLatin1String("duduregi"), Qt::CaseInsensitive) == 0)
        return new QDuduregiPlatformInputContext;

    return 0;
}

QT_END_NAMESPACE

#include "main.moc"
