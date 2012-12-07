/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef JSNotificationCenter_h
#define JSNotificationCenter_h

#if ENABLE(NOTIFICATIONS)

#include "JSDOMBinding.h"
#include <runtime/JSGlobalObject.h>
#include <runtime/JSObjectWithGlobalObject.h>
#include <runtime/ObjectPrototype.h>

namespace WebCore {

class NotificationCenter;

class JSNotificationCenter : public JSDOMWrapper {
    typedef JSDOMWrapper Base;
public:
    JSNotificationCenter(JSC::Structure*, JSDOMGlobalObject*, PassRefPtr<NotificationCenter>);
    static JSC::JSObject* createPrototype(JSC::ExecState*, JSC::JSGlobalObject*);
    static const JSC::ClassInfo s_info;

    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), AnonymousSlotCount, &s_info);
    }


    // Custom functions
    JSC::JSValue requestPermission(JSC::ExecState*);
    NotificationCenter* impl() const { return m_impl.get(); }

private:
    RefPtr<NotificationCenter> m_impl;
protected:
    static const unsigned StructureFlags = Base::StructureFlags;
};

class JSNotificationCenterOwner : public JSC::WeakHandleOwner {
    virtual bool isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown>, void* context, JSC::SlotVisitor&);
    virtual void finalize(JSC::Handle<JSC::Unknown>, void* context);
};

inline JSC::WeakHandleOwner* wrapperOwner(DOMWrapperWorld*, NotificationCenter*)
{
    DEFINE_STATIC_LOCAL(JSNotificationCenterOwner, jsNotificationCenterOwner, ());
    return &jsNotificationCenterOwner;
}

inline void* wrapperContext(DOMWrapperWorld* world, NotificationCenter*)
{
    return world;
}

JSC::JSValue toJS(JSC::ExecState*, JSDOMGlobalObject*, NotificationCenter*);
NotificationCenter* toNotificationCenter(JSC::JSValue);

class JSNotificationCenterPrototype : public JSC::JSObjectWithGlobalObject {
    typedef JSC::JSObjectWithGlobalObject Base;
public:
    static JSC::JSObject* self(JSC::ExecState*, JSC::JSGlobalObject*);
    static const JSC::ClassInfo s_info;
    virtual bool getOwnPropertySlot(JSC::ExecState*, const JSC::Identifier&, JSC::PropertySlot&);
    virtual bool getOwnPropertyDescriptor(JSC::ExecState*, const JSC::Identifier&, JSC::PropertyDescriptor&);
    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), AnonymousSlotCount, &s_info);
    }
    JSNotificationCenterPrototype(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::Structure* structure) : JSC::JSObjectWithGlobalObject(globalData, globalObject, structure) { }
protected:
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | Base::StructureFlags;
};

// Functions

JSC::EncodedJSValue JSC_HOST_CALL jsNotificationCenterPrototypeFunctionCreateHTMLNotification(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsNotificationCenterPrototypeFunctionCreateNotification(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsNotificationCenterPrototypeFunctionCheckPermission(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsNotificationCenterPrototypeFunctionRequestPermission(JSC::ExecState*);

} // namespace WebCore

#endif // ENABLE(NOTIFICATIONS)

#endif
