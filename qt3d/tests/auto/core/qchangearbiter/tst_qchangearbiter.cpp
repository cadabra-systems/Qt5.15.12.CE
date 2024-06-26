/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

// TODO Remove in Qt6
#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED

#include <QtTest/QTest>
#include <Qt3DCore/private/qchangearbiter_p.h>
#include <Qt3DCore/private/qscene_p.h>
#include <Qt3DCore/qnode.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qcomponent.h>
#include <Qt3DCore/qbackendnode.h>
#include <Qt3DCore/private/qnode_p.h>
#include <Qt3DCore/private/qbackendnode_p.h>
#include <QThread>
#include <QWaitCondition>
#include "testpostmanarbiter.h"

class tst_QChangeArbiter : public QObject
{
    Q_OBJECT

private slots:
    void recordsDirtyNodes();
};

// used to test property change notifications
class PropertyTestNode : public Qt3DCore::QNode
{
    Q_OBJECT
    Q_PROPERTY(int prop1 READ prop1 WRITE setProp1 NOTIFY prop1Changed)
    Q_PROPERTY(float prop2 READ prop2 WRITE setProp2 NOTIFY prop2Changed)

public:
    explicit PropertyTestNode(Qt3DCore::QNode *parent = 0) : Qt3DCore::QNode(parent)
    {}

    int prop1() const { return m_prop1; }
    void setProp1(int v)
    {
        if (m_prop1 != v) {
            m_prop1 = v;
            Q_EMIT prop1Changed(v);
        }
    }

    float prop2() const { return m_prop2; }
    void setProp2(float v)
    {
        if (m_prop2 != v) {
            m_prop2 = v;
            Q_EMIT prop2Changed(v);
        }
    }

Q_SIGNALS:
    void prop1Changed(int v);
    void prop2Changed(float v);

private:
    int m_prop1 = 0;
    float m_prop2 = 0.0f;
};

void tst_QChangeArbiter::recordsDirtyNodes()
{
    // GIVEN
    QScopedPointer<TestArbiter> arbiter(new TestArbiter());
    QScopedPointer<Qt3DCore::QScene> scene(new Qt3DCore::QScene());
//    arbiter->setScene(scene.data());
    scene->setArbiter(arbiter.data());

    // WHEN
    auto *root = new PropertyTestNode();
    auto *child = new PropertyTestNode(root);

    root->setProp1(883);
    child->setProp2(1584);

    // THEN
    QCOMPARE(arbiter->dirtyNodes.size(), 0);

    // WHEN
    Qt3DCore::QNodePrivate::get(root)->setArbiter(arbiter.data());
    root->setProp1(884);
    child->setProp2(1585);

    // THEN
    QCOMPARE(arbiter->dirtyNodes.size(), 1);
    QCOMPARE(arbiter->dirtyNodes.front(), root);

    arbiter->dirtyNodes.clear();

    // WHEN
    Qt3DCore::QNodePrivate::get(child)->setArbiter(arbiter.data());
    child->setProp2(1586);

    // THEN
    QCOMPARE(arbiter->dirtyNodes.size(), 1);
    QCOMPARE(arbiter->dirtyNodes.front(), child);

    arbiter->dirtyNodes.clear();

    // WHEN
    root->setProp1(887);
    child->setProp2(1587);

    // THEN
    QCOMPARE(arbiter->dirtyNodes.size(), 2);
}

QTEST_MAIN(tst_QChangeArbiter)

#include "tst_qchangearbiter.moc"
