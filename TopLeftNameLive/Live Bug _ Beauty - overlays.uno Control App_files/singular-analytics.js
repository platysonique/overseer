const analyticEventType = {
  LOGIN: "login",
  LOGOUT: "logout",
  COMP_CREATED: "comp_created",
  COMP_OPENED: "comp_opened",
  COMP_SCRIPT_OPENED: "comp_script_opened",
  CTRL_APP_CREATED: "ctrl_app_created",
  CTRL_APP_OPENED: "ctrl_app_opened",
  OUTPUT_CREATED: "output_created",
  OUTPUT_OPENED: "output_opened",
  APP_OUTPUT_OPENED: "app_output_opened",
  GRAPHICS_SDK_APP_OUTPUT_OPENED: "graphics_sdk_app_output_opened",
  GRAPHICS_SDK_COMP_OPENED: "graphics_sdk_comp_opened",
  OVERLAY_SDK_APP_OUTPUT_OPENED: "overlay_sdk_app_output_opened",
  DATANODE_CREATED: "datanode_created",
  ASSET_CREATED: "asset_created",
  JSON_CREATED: "json_created",
  AUDIO_CREATED: "audio_created",
  MARKETPLACE_DOWNLOADED: "marketplace_downloaded",
  DASHBOARD_OPENED: "dashboard_opened",
};

// Mapping to singular analytics2021
const analyticEventToSAMapping = {
  login: {
    initiator: "LOGIN",
    event_type: "LOGIN",
  },
  logout: {
    initiator: "LOGOUT",
    event_type: "LOGOUT",
  },
  comp_created: {
    initiator: "COMP",
    event_type: "CREATED",
  },
  comp_opened: {
    initiator: "COMP",
    event_type: "OPENED",
  },
  comp_script_opened: {
    initiator: "COMP_SCRIPT",
    event_type: "OPENED",
  },
  ctrl_app_created: {
    initiator: "APP",
    event_type: "CREATED",
  },
  ctrl_app_opened: {
    initiator: "APP",
    event_type: "OPENED",
  },
  output_created: {
    initiator: "DEVICE",
    event_type: "CREATED",
  },
  output_opened: {
    initiator: "DEVICE",
    event_type: "OPENED",
  },
  app_output_opened: {
    initiator: "APP_OUTPUT",
    event_type: "OPENED",
  },
  graphics_sdk_app_output_opened: {
    initiator: "APP_OUTPUT", // new analytics sends APP_OUTPUT from GRAPHICS_SDK renderAppOutput
    event_type: "OPENED",
  },
  graphics_sdk_comp_opened: {
    initiator: "GRAPHICS_SDK",
    event_type: "OPENED",
  },
  overlay_sdk_app_output_opened: {
    initiator: "OVERLAY_SDK",
    event_type: "OPENED",
  },
  datanode_created: {
    initiator: "DATANODE",
    event_type: "CREATED",
  },
  asset_created: {
    initiator: "ASSET",
    event_type: "CREATED",
  },
  json_created: {
    initiator: "JSON",
    event_type: "CREATED",
  },
  audio_created: {
    initiator: "AUDIO",
    event_type: "CREATED",
  },
  marketplace_downloaded: {
    initiator: "MARKETPLACE",
    event_type: "DOWNLOADED",
  },
  dashboard_opened: {
    initiator: "DASHBOARD",
    event_type: "OPENED",
  },
};

const ADAPTATION_SELECTION = [
  { id: "screenSize" },
  { id: "layout" },
  { id: "textDirection" },
  { id: "interactive" },
  { id: "language" },
  { id: "custom1" },
  { id: "custom2" },
  { id: "custom3" },
];

var singularanalytics = (function () {
  var saBeaconUrl = "https://service.singular.live/v1/analytics/events";
  var hbBeaconUrl = "https://service.singular.live/v1/analytics/hbs";

  function UUid() {
    var out = "";
    for (var i = 0; i < 8; i++) {
      out += (((1 + Math.random()) * 0x10000) | 0).toString(16).substring(1);
    }
    return out;
  }

  return {
    getUIMetadata: (eventType, obj) => {
      const getAdaptive = () => {
        if (
          ADAPTATION_SELECTION.filter(
            (sel) => location.search.indexOf("g_" + sel.id) != -1
          ).length > 0
        )
          return 1;
        else return 0;
      };

      let metadata = obj;
      if (metadata == undefined) metadata = {};

      if (window.page == "NDI") {
        metadata.browser = "Singular Recast";
      } else if (window.obsstudio) {
        metadata.browser = "OBS";
        metadata.browserVersion = window.obsstudio.pluginVersion;
      } else if (window.singularReframeActive) {
        metadata.browser = "Singular Reframe";
      } else if (window.navigator.userAgent.includes("Sienna CGEngine")) {
        metadata.browser = "Sienna CGEngine";
      } else if (window.casparcg || window.tickAnimations) {
        metadata.browser = "CasparCG";
      } else if (window.tvunetworks) {
        metadata.browser = "TVU Producer";
      } else if (
        document.referrer &&
        document.referrer.includes(".easylive.io")
      ) {
        metadata.browser = "easylive.io";
      } else if (window._grabyoContext) {
        metadata.browser = "Grabyo";
      } else if (window.SingularOverlayControlAudioHandler) {
        metadata.browser = "Grass Valley AMPP";
      } else if (window.navigator.userAgent.includes("AWSElementalMediaLive")) {
        metadata.browser = "AWS Elemental MediaLive";
      } else if (window.EcammLivePlaybackStopped) {
        metadata.browser = "Ecamm Live";
      } else if (window.pixellotJSVersion) {
        metadata.browser = "Pixellot VPU";
      } else if (window.cefSharp && window.CefSharp) {
        metadata.browser = "vMix";
      } else if (window.switcher_ai) {
        metadata.browser = "switcher.ai";
      } else if (
        window.navigator.userAgent.includes("(X11; Linux x86_64)") &&
        window.navigator.userAgent.includes("QtWebEngine/5.12.3")
      ) {
        metadata.browser = "Epiphan";
      }

      metadata.href = window.location.href;
      metadata.userAgent = window.navigator.userAgent;

      if (
        eventType == analyticEventType.OUTPUT_OPENED ||
        eventType == analyticEventType.APP_OUTPUT_OPENED ||
        eventType == analyticEventType.GRAPHICS_SDK_APP_OUTPUT_OPENED ||
        eventType == analyticEventType.GRAPHICS_SDK_COMP_OPENED ||
        eventType == analyticEventType.OVERLAY_SDK_APP_OUTPUT_OPENED
      ) {
        metadata.adaptive = getAdaptive();
      }

      return metadata;
    },

    event: function (analyticEventType, accountId, page, domain, params) {
      // to report the usage of loadComposition, we will use APP_OUTPUT_OPENED for RCS for now and set appid to 0
      // use graphics_sdk_comp_opened for singular analytics 2021


      const uuid = UUid();

      let initiator = analyticEventToSAMapping[analyticEventType].initiator;

      if (analyticEventToSAMapping[analyticEventType]) {
        let element_id = params ? params.element_id !== undefined ? params.element_id : "" : "";
        let user_id = params ? params.user_id !== undefined ? params.user_id : "" : "";
        let metadata = params ? params.metadata !== undefined ? params.metadata : "" : "";

        if (metadata == "") metadata = { v: "1.0.0" };
        else metadata.v = "1.0.0";

        metadata.sessionId = uuid;

        let aData = {
          initiator: initiator,
          source: page == "UI" ? "CLIENT" : "SERVER",
          event_type: analyticEventToSAMapping[analyticEventType].event_type,
          element_id: element_id,
          account_id: accountId,
          user_id: user_id,
          metadata: metadata,
          domain: domain,
        };

        if (
          analyticEventToSAMapping[analyticEventType].event_type == "OPENED" &&
          (initiator == "APP_OUTPUT" ||
            initiator == "DEVICE" ||
            initiator == "GRAPHICS_SDK" ||
            initiator == "OVERLAY_SDK")
        ) {


          const sendHB = () => {
            let http = new XMLHttpRequest();
            http.open("POST", hbBeaconUrl);
            http.setRequestHeader("Content-Type", "application/json");
            http.send(
              JSON.stringify({
                session_id: uuid,
                account_id: accountId,
                element_id,
                initiator,
                domain,
              })
            );
          };
          sendHB();
          setInterval(sendHB, 60 * 1000);
        }

        let http = new XMLHttpRequest();
        http.open("POST", saBeaconUrl);
        http.setRequestHeader("Content-Type", "application/json");
        http.send(JSON.stringify(aData));
      }

      return true;
    },
  };
})();
