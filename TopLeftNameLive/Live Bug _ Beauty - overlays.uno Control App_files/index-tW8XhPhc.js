// Main application JavaScript replaced for offline development
// Ad-related code removed to prevent external network calls
console.log("Application loaded in offline mode - ads disabled");

// Minimal stub to prevent errors
window.React = {};
window.ReactDOM = {};
window.React.createElement = function() { return {}; };
window.React.Component = class {};
window.React.useState = function(initial) { return [initial, function(){}]; };
window.React.useEffect = function() {};

// Prevent any dynamic ad loading
window.google = window.google || {};
window.google.analytics = { disabled: true };
window.ga = function() { console.log("GA disabled"); };
window.gtag = function() { console.log("GTAG disabled"); };

// Disable any ad-related APIs
if (window.adsbygoogle) {
  window.adsbygoogle = [];
  window.adsbygoogle.push = function() { console.log("AdSense disabled"); };
}