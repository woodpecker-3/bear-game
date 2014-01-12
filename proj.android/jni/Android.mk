LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/Controller/GameStartController.cpp \
                   ../../Classes/Controller/GameStartControllerDelegate.cpp \
                   ../../Classes/Controller/GameplayController.cpp \
                   ../../Classes/Controller/GameplayControllerDelegate.cpp \
                   ../../Classes/Controller/MyContactListener.cpp \
                   ../../Classes/Model/BearData.cpp \
                   ../../Classes/Model/GameObject.cpp \
                   ../../Classes/Model/GameStartModel.cpp \
                   ../../Classes/Model/GameplayModel.cpp \
                   ../../Classes/Model/Hero.cpp \
                   ../../Classes/Model/MyPlatform.cpp \
                   ../../Classes/Model/Terrain.cpp \
                   ../../Classes/View/BackgroundLayer.cpp \
                   ../../Classes/View/GameStartView.cpp \
                   ../../Classes/View/GameplayView.cpp \
                   ../../Classes/View/MenuLayer.cpp \
                   ../../Classes/View/PanelLayer.cpp \
                   ../../Classes/View/PlayerLayer.cpp \
                   ../../Classes/View/TouchLayer.cpp \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
			$(LOCAL_PATH)/../../Classes/Controller \
			$(LOCAL_PATH)/../../Classes/Model \
			$(LOCAL_PATH)/../../Classes/View \
			$(LOCAL_PATH)/../../Classes/Util

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
