/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */


#include <qpa/qplatforminputcontextplugin_p.h>
#include <QtCore/QStringList>
#include "qwrscompositorplatforminputcontext.h"

QT_BEGIN_NAMESPACE

class QWrsCompositorPlatformInputContextPlugin : public QPlatformInputContextPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QPlatformInputContextFactoryInterface_iid FILE "wrscompositor.json")

public:
    QWrsCompositorPlatformInputContext *create(const QString&, const QStringList&) Q_DECL_OVERRIDE;
};

QWrsCompositorPlatformInputContext *QWrsCompositorPlatformInputContextPlugin::create(const QString& system, const QStringList& paramList)
{
    Q_UNUSED(paramList);

    if (system.compare(system, QLatin1String("wrscompositor"), Qt::CaseInsensitive) == 0)
        return new QWrsCompositorPlatformInputContext;

    return 0;
}

QT_END_NAMESPACE

#include "main.moc"
