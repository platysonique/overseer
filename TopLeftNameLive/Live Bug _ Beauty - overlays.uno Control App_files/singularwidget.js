/*!
    Singular Widget Library
    @name singularwidget.js
    @author Singular Team
    @version 1.0.4
    @date 10/24/2025
    @copyright (c) 2025 Singular.Live PTE
    @license Licensed under the MIT (http://www.opensource.org/licenses/mit-license.php) license.
*/

// Independent Singular object to be used in widgets development
(function () {

  /** Used to determine if values are of the language type `Object`. */
  var objectTypes = {
    'function': true,
    'object': true
  };

  /** Detect free variable `exports`. */
  var freeExports = objectTypes[typeof exports] && exports && !exports.nodeType && exports;

  /** Detect free variable `module`. */
  var freeModule = objectTypes[typeof module] && module && !module.nodeType && module;

  /** Detect free variable `global` from Node.js. */
  var freeGlobal = freeExports && freeModule && typeof global == 'object' && global;

  /** Detect free variable `self`. */
  var freeSelf = objectTypes[typeof self] && self && self.Object && self;

  /** Detect free variable `window`. */
  var freeWindow = objectTypes[typeof window] && window && window.Object && window;

  /**
  * Used as a reference to the global object.
  *
  * The `this` value is used if it is the global object to avoid Greasemonkey's
  * restricted `window` object, otherwise the `window` object is used.
  */
  var root = freeGlobal || ((freeWindow !== (this && this.window)) && freeWindow) || freeSelf || this;

  root.SingularWidget = (function () {

    var extend = function () {
      // Variables
      var extended = {};
      var deep = false;
      var i = 0;
      var length = arguments.length;

      // Check if a deep merge
      if (Object.prototype.toString.call(arguments[0]) === '[object Boolean]') {
        deep = arguments[0];
        i++;
      }

      // Merge the object into the extended object
      var merge = function (obj) {
        for (var prop in obj) {
          if (Object.prototype.hasOwnProperty.call(obj, prop)) {
            // If deep merge and property is an object, merge properties
            if (deep && Object.prototype.toString.call(obj[prop]) === '[object Object]') {
              extended[prop] = extend(true, extended[prop], obj[prop]);
            } else {
              extended[prop] = obj[prop];
            }
          }
        }
      };

      // Loop through each object and conduct a merge
      for (; i < length; i++) {
        var obj = arguments[i];
        merge(obj);
      }

      return extended;

    }

    var compareObject = function (x, y) {
      if (x === y) return true;
      // if both x and y are null or undefined and exactly the same

      if (!(x instanceof Object) || !(y instanceof Object)) return false;
      // if they are not strictly equal, they both need to be Objects

      if (x.constructor !== y.constructor) return false;
      // they must have the exact same prototype chain, the closest we can do is
      // test there constructor.

      for (var p in x) {
        if (!x.hasOwnProperty(p)) continue;
        // other properties were tested using x.constructor === y.constructor

        if (!y.hasOwnProperty(p)) return false;
        // allows to compare x[ p ] and y[ p ] when set to undefined

        if (x[p] === y[p]) continue;
        // if they have the same strict value or identity then they are equal

        if (typeof (x[p]) !== "object") return false;
        // Numbers, Strings, Functions, Booleans must be strictly equal

        if (!compareObject(x[p], y[p])) return false;
        // Objects and Arrays must be tested recursively
      }

      for (p in y) {
        if (y.hasOwnProperty(p) && !x.hasOwnProperty(p)) return false;
        // allows x[ p ] to be set to undefined
      }
      return true;
    }

    var _styles,

      _onInit,
      _onValue,
      _onButtonClicked,
      _onEditComp,
      _onAnimation,
      _onAdaptationGlobalsChanged,
      _onStateChanged,
      _bodyInlineCssBackup, // Keep user-defined style in body before override by Singular style

      prevState; // keep previous onStateChanged param

    // _domTargetNumber = 0,

    window.addEventListener('message', function (event) {

      // IMPORTANT: Check the origin of the data!
      // if (~event.origin.indexOf('http://yoursite.com')) {
      //     // The data has been sent from your site

      //     // The data sent with postMessage is stored in event.data
      //     console.log(event.data);
      // } else {
      //     // The data hasn't been sent from your site!
      //     // Be careful! Do not use it.
      //     return;
      // }
      var onInit = event.data._singular_onInit;
      if (!onInit) onInit = event.data.onInit; // for old version

      var onValue = event.data._singular_onValue;
      if (!onValue) onValue = event.data.onValue; // for old version

      var onButtonClicked = event.data._singular_onButtonClicked;
      if (!onButtonClicked) onButtonClicked = event.data.onButtonClicked; // for old version

      var onElementStyle = event.data._singular_onElementStyle;
      if (!onElementStyle) onElementStyle = event.data.onElementStyle; // for old version

      var onStyles = event.data._singular_onStyles;
      if (!onStyles) onStyles = event.data.onStyles; // for old version

      var onEditComp = event.data._singular_onEditComp;
      if (!onEditComp) onEditComp = event.data.onEditComp; // for old version

      var onAnimation = event.data._singular_onAnimation;
      if (!onAnimation) onAnimation = event.data.onAnimation; // for old version

      var onAdaptationGlobalsChanged = event.data._singular_onAdaptationGlobalsChanged;
      var onStateChanged = event.data._singular_onStateChanged;

      if (onInit) {
        SingularWidget._styles = [];
        SingularWidget.tileId = onInit.tileId;
        SingularWidget.tileName = onInit.tileName;
        SingularWidget.compositionId = onInit.compositionId;
        SingularWidget.instanceId = onInit.instanceId;
        SingularWidget.widgetId = onInit.widgetId;
        SingularWidget.widgetVersion = onInit.widgetVersion;

        if (SingularWidget._onInit) {
          // Tell widget that we're ready
          SingularWidget._onInit(onInit.params);
        }
      } else if (onValue) {
        if (SingularWidget._onValue) {
          SingularWidget._onValue(onValue);
        }
      } else if (onButtonClicked) {
        if (SingularWidget._onButtonClicked) {
          SingularWidget._onButtonClicked(onButtonClicked);
        }
      } else if (onElementStyle) {

        // Apply style to body
        if (onElementStyle != 'clear') {
          for (key in onElementStyle) {
            //$(document.body).css(key, event.data.onElementStyle[key]);
          }
        } else {
          //$(document.body).attr('style', SingularWidget._bodyInlineCssBackup);
        }
      } else if (onStyles) {

        SingularWidget._styles = onStyles;

      } else if (onEditComp) {
        if (SingularWidget._onEditComp) {
          var previewObj = SingularWidget._onEditComp(onEditComp);

          var targetFunction = findFunction('compEditorMessage');
          if (targetFunction)
            targetFunction({ message: 'edit-comp-standalone', params: previewObj });

        }
      } else if (onAnimation) {
        if (SingularWidget._onAnimation) {
          SingularWidget._onAnimation(onAnimation);
        }
      } else if (onAdaptationGlobalsChanged) {
        if (SingularWidget._onAdaptationGlobalsChanged) {
          SingularWidget._onAdaptationGlobalsChanged(onAdaptationGlobalsChanged);
        }
      } else if (onStateChanged) {
        if (SingularWidget._onStateChanged) {
          if (!compareObject(onStateChanged, SingularWidget.prevState)) {
            SingularWidget._onStateChanged(onStateChanged, SingularWidget.prevState)
            SingularWidget.prevState = extend(true, onStateChanged);
          }
        }
      }
    });

    var init = function (hash) {
      if (hash.onInit) {
        this._onInit = hash.onInit;

        // If we're ready then trigger it immediately
        if (this._styles) {
          this._onInit();
        }
      }

      if (hash.onValue) {
        this._onValue = hash.onValue;
      }

      if (hash.onButtonClicked) {
        this._onButtonClicked = hash.onButtonClicked;
      }

      if (hash.onEditComp) {
        this._onEditComp = hash.onEditComp;
      }

      if (hash.onAnimation) {
        this._onAnimation = hash.onAnimation;
      }

      if (hash.onAdaptationGlobalsChanged) {
        this._onAdaptationGlobalsChanged = hash.onAdaptationGlobalsChanged;
      }

      if (hash.onStateChanged) {
        this._onStateChanged = hash.onStateChanged;
      }

      //this._bodyInlineCssBackup = $(document.body).attr('style');
      //document.domain = 'singular.live';
    }

    var getStyle = function (id) {
      var style = this._styles.filter(function (s) {
        return s.id == id;
      });

      if (style.length) {
        return style[0];
      } else {
        return null;
      }
    }

    var setCustomWidgetUI = function (customModel, cleanupOldPayload, updatePayload) {
      var targetFunction = findFunction('setCustomWidgetUI');
      if (targetFunction)
        return targetFunction(SingularWidget.tileId, 
          SingularWidget.compositionId,
          SingularWidget.widgetId,
          SingularWidget.widgetVersion,
          customModel,
          cleanupOldPayload,
          updatePayload);
    }

    // provide funtions that widget can call.
    /****************************************************************************************************************/

    //
    var findFunction = function (functionName) {
      var parent = window.parent;
      var count = 0;
      var maxCount = 100;

      while (parent && typeof parent[functionName] !== 'function' && count < maxCount) {
        parent = parent.parent;
        count++;
      }

      if (count >= maxCount) {
        console.warn('Function [' + functionName + '] is not found');
        return function () { };
      }

      return parent[functionName];
    }
    //

    var instantiate = function (compName, domTarget, instantiateCallback) {
      var targetFunction = findFunction('instantiate');
      if (targetFunction)
        return targetFunction(compName, domTarget, instantiateCallback, window.name, SingularWidget.tileId, SingularWidget.compositionId, SingularWidget.instanceId);
    }

    var importComposition = function (url, importCallback) {
      var targetFunction = findFunction('importComposition');
      if (targetFunction)
        return targetFunction(url, importCallback);

    }

    var importCompositionFromJSON = function (compJSON, importCallback) {
      var targetFunction = findFunction('importCompositionFromJSON');
      if (targetFunction)
        return targetFunction(compJSON, importCallback);
    }

    // remove from sbase/firebase
    var removeComposition = function (compId) {
      var targetFunction = findFunction('removeComposition');
      if (targetFunction)
        return targetFunction(compId);
    }

    var sendCustomMessage = function (obj) {
      var targetFunction = findFunction('sendCustomMessage');
      if (targetFunction)
        return targetFunction(obj, window.name, SingularWidget.tileId, SingularWidget.tileName, SingularWidget.compositionId, SingularWidget.instanceId, SingularWidget.widgetId);
    }

    var logFuntions = {
      info: function (params) {
        var targetFunction = findFunction('compEditorMessage');
        if (targetFunction)
          return targetFunction({ message: 'widget-log', params: { type: 'INFO', log: params, tileId: SingularWidget.tileId, compositionId: SingularWidget.compositionId, instanceId: SingularWidget.instanceId } });
      },

      warn: function (params) {
        var targetFunction = findFunction('compEditorMessage');
        if (targetFunction)
          return targetFunction({ message: 'widget-log', params: { type: 'WARNING', log: params, tileId: SingularWidget.tileId, compositionId: SingularWidget.compositionId, instanceId: SingularWidget.instanceId } });
      },

      error: function (params) {
        var targetFunction = findFunction('compEditorMessage');
        if (targetFunction)
          return targetFunction({ message: 'widget-log', params: { type: 'ERROR', log: params, tileId: SingularWidget.tileId, compositionId: SingularWidget.compositionId, instanceId: SingularWidget.instanceId } });
      },
    }

    var createTimeControl = function () {
      return (function () {
        return {
          tc: {
            UTC: 0,
            isrunning: false,
            value: 0
          },

          allowBackwardsJump: false,
          roundToInterval: true,

          intervalID: null,
          timeoutID: null,
          oldRunningTime: 0,
          intervalTime: 1000,
          offsetToServerTime: 0,
          updateCallback: null,

          setOffsetToServerTime: function (time) {
            if (this._isNumeric(time)) {
              this.offsetToServerTime = parseFloat(time);
            }
          },

          setIntervalTime: function (time) {
            if (this._isNumeric(time)) {
              let t = parseFloat(time);
              if (t != this.intervalTime) {
                this.intervalTime = parseFloat(time);
                this._clearInterval();
                this._startInterval();
              }
            }
          },

          setUpdateCallback: function (callback) {
            this.updateCallback = callback;
          },

          setAllowBackwardsJump: function (v) {
            this.allowBackwardsJump = v;
          },

          setRoundToInterval: function (v) {
            this.roundToInterval = v;
          },

          setTimeControl: function (tc) {
            // we received a string. This might be a JSON so convert it into an object
            if (typeof tc === 'string' || tc instanceof String) {
              try {
                tc = JSON.parse(tc);
              } catch (e) {}
            }

            // check if tc is actually a timecontrol object
            if (tc === undefined || tc.value === undefined || tc.UTC === undefined || tc.isRunning === undefined) {
              return;
            }

            if (tc.value != this.tc.value || tc.UTC != this.tc.UTC || tc.isRunning != this.tc.isRunning) {
              // the clock was reset. Also start my local running time at 0
              if (tc.value <= this.tc.value) {
                this.oldRunningTime = 0;
              }

              if (this.tc.isRunning != tc.isRunning) {
                // remember tc already here, because we will need it in the functions below
                this.tc = tc;

                this._clearInterval();

                // start the timeout
                if (this.tc.isRunning) {
                  this._startInterval();
                  return;
                }
              } else {
                this.tc = tc;
              }
              this._executeUpdateCallback();
            }
          },

          getCurrentTime: function () {
            var runningTime = this.tc.value;
            if (this.tc.isRunning) {
              // we take the local time and subtract the last UTC.
              runningTime += Date.now() + this.offsetToServerTime - this.tc.UTC;
            }

            // we do not allow the time to run backwards. This can happen when the clock is stopped, because
            // the local time keeps on running while the command to stop the clock is sent. 
            // That would cause the clock to jump backwards a bit, which looks really stupid.
            // but this is a real problem in case you want to show an exact time.
            if (!this.allowBackwardsJump) {
              if (runningTime < this.oldRunningTime) {
                runningTime = this.oldRunningTime;
              } else {
                this.oldRunningTime = runningTime;
              }
            }

            // round the time we display to the update interval. This makes the time we show much more stable
            let correction = 0;
            if (this.roundToInterval) {
              var remainder = runningTime % this.intervalTime;
              if (remainder > this.intervalTime / 2) {
                correction = this.intervalTime - remainder;
              } else {
                correction = -remainder;
              }
            }
            return runningTime + correction;
          },

          _clearInterval: function () {
            // clear the interval that exists
            if (this.intervalID) {
              clearInterval(this.intervalID);
              this.intervalID = null;
            }

            // clear the timeout
            if (this.timeoutID) {
              clearTimeout(this.timeoutID);
              this.timeoutID = null;
            }
          },

          _startInterval: function () {
            // now caclulate the remainder with the running time and interval
            // so that our setInterval below always hits at exactly that right time
            let remainder = this.getCurrentTime() % this.intervalTime;

            // stop the interval and timeout
            this._clearInterval();

            // create a timeout to wait for the difference between interval time and remainder.
            // we try to get the timeout to run exactly on the time where the intervaltime changes.
            // e.g. for 1sec the interval should execute so that the time in the updateSubCompInstance function
            // is as close to the second as possible. For 0.1sec it doesn't really matter, for sec also, but minute is important.
            this.timeoutID = setTimeout(function () {
              this._executeUpdateCallback();

              // create the new interval
              this.intervalID = setInterval(function () {
                this._executeUpdateCallback();
              }.bind(this), this.intervalTime);
            }.bind(this), this.intervalTime - remainder);
          },

          _executeUpdateCallback: function () {
            if (this.updateCallback) {
              this.updateCallback();
            }
          },

          _isNumeric: function isNumeric(n) {
            return !isNaN(parseFloat(n)) && isFinite(n);
          },
        };
      }());
    }

    /****************************************************************************************************************/
    return {
      init: init,
      setCustomWidgetUI: setCustomWidgetUI,
      getStyle: getStyle,
      instantiate: instantiate,
      importComposition: importComposition,
      importCompositionFromJSON: importCompositionFromJSON,
      removeComposition: removeComposition,
      sendCustomMessage: sendCustomMessage,
      log: logFuntions,
      createTimeControl: createTimeControl
    };
  })();

}.call(this));
