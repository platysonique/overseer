/*!
		Singular Player Library
		@name singularplayer.js
		@author Singular Team
		@version 0.1.2
		@date 02/28/2017
		@copyright (c) 2016 Singular PTE.
		@license Licensed under the MIT (http://www.opensource.org/licenses/mit-license.php) license.
*/

!function (t) { function e(t, n) { if (t = t ? t : "", n = n || {}, t instanceof e) return t; if (!(this instanceof e)) return new e(t, n); var a = r(t); this._originalInput = t, this._r = a.r, this._g = a.g, this._b = a.b, this._a = a.a, this._roundA = $(100 * this._a) / 100, this._format = n.format || a.format, this._gradientType = n.gradientType, this._r < 1 && (this._r = $(this._r)), this._g < 1 && (this._g = $(this._g)), this._b < 1 && (this._b = $(this._b)), this._ok = a.ok, this._tc_id = P++ } function r(t) { var e = { r: 0, g: 0, b: 0 }, r = 1, a = null, s = null, f = null, h = !1, l = !1; return "string" == typeof t && (t = E(t)), "object" == typeof t && (z(t.r) && z(t.g) && z(t.b) ? (e = n(t.r, t.g, t.b), h = !0, l = "%" === String(t.r).substr(-1) ? "prgb" : "rgb") : z(t.h) && z(t.s) && z(t.v) ? (a = I(t.s), s = I(t.v), e = o(t.h, a, s), h = !0, l = "hsv") : z(t.h) && z(t.s) && z(t.l) && (a = I(t.s), f = I(t.l), e = i(t.h, a, f), h = !0, l = "hsl"), t.hasOwnProperty("a") && (r = t.a)), r = S(r), { ok: h, format: t.format || l, r: D(255, U(e.r, 0)), g: D(255, U(e.g, 0)), b: D(255, U(e.b, 0)), a: r } } function n(t, e, r) { return { r: 255 * H(t, 255), g: 255 * H(e, 255), b: 255 * H(r, 255) } } function a(t, e, r) { t = H(t, 255), e = H(e, 255), r = H(r, 255); var n, a, i = U(t, e, r), s = D(t, e, r), o = (i + s) / 2; if (i == s) n = a = 0; else { var f = i - s; switch (a = o > .5 ? f / (2 - i - s) : f / (i + s), i) { case t: n = (e - r) / f + (r > e ? 6 : 0); break; case e: n = (r - t) / f + 2; break; case r: n = (t - e) / f + 4 }n /= 6 } return { h: n, s: a, l: o } } function i(t, e, r) { function n(t, e, r) { return 0 > r && (r += 1), r > 1 && (r -= 1), 1 / 6 > r ? t + 6 * (e - t) * r : .5 > r ? e : 2 / 3 > r ? t + (e - t) * (2 / 3 - r) * 6 : t } var a, i, s; if (t = H(t, 360), e = H(e, 100), r = H(r, 100), 0 === e) a = i = s = r; else { var o = .5 > r ? r * (1 + e) : r + e - r * e, f = 2 * r - o; a = n(f, o, t + 1 / 3), i = n(f, o, t), s = n(f, o, t - 1 / 3) } return { r: 255 * a, g: 255 * i, b: 255 * s } } function s(t, e, r) { t = H(t, 255), e = H(e, 255), r = H(r, 255); var n, a, i = U(t, e, r), s = D(t, e, r), o = i, f = i - s; if (a = 0 === i ? 0 : f / i, i == s) n = 0; else { switch (i) { case t: n = (e - r) / f + (r > e ? 6 : 0); break; case e: n = (r - t) / f + 2; break; case r: n = (t - e) / f + 4 }n /= 6 } return { h: n, s: a, v: o } } function o(e, r, n) { e = 6 * H(e, 360), r = H(r, 100), n = H(n, 100); var a = t.floor(e), i = e - a, s = n * (1 - r), o = n * (1 - i * r), f = n * (1 - (1 - i) * r), h = a % 6, l = [n, o, s, s, f, n][h], u = [f, n, n, o, s, s][h], c = [s, s, f, n, n, o][h]; return { r: 255 * l, g: 255 * u, b: 255 * c } } function f(t, e, r, n) { var a = [M($(t).toString(16)), M($(e).toString(16)), M($(r).toString(16))]; return n && a[0].charAt(0) == a[0].charAt(1) && a[1].charAt(0) == a[1].charAt(1) && a[2].charAt(0) == a[2].charAt(1) ? a[0].charAt(0) + a[1].charAt(0) + a[2].charAt(0) : a.join("") } function h(t, e, r, n, a) { var i = [M($(t).toString(16)), M($(e).toString(16)), M($(r).toString(16)), M(L(n))]; return a && i[0].charAt(0) == i[0].charAt(1) && i[1].charAt(0) == i[1].charAt(1) && i[2].charAt(0) == i[2].charAt(1) && i[3].charAt(0) == i[3].charAt(1) ? i[0].charAt(0) + i[1].charAt(0) + i[2].charAt(0) + i[3].charAt(0) : i.join("") } function l(t, e, r, n) { var a = [M(L(n)), M($(t).toString(16)), M($(e).toString(16)), M($(r).toString(16))]; return a.join("") } function u(t, r) { r = 0 === r ? 0 : r || 10; var n = e(t).toHsl(); return n.s -= r / 100, n.s = R(n.s), e(n) } function c(t, r) { r = 0 === r ? 0 : r || 10; var n = e(t).toHsl(); return n.s += r / 100, n.s = R(n.s), e(n) } function g(t) { return e(t).desaturate(100) } function b(t, r) { r = 0 === r ? 0 : r || 10; var n = e(t).toHsl(); return n.l += r / 100, n.l = R(n.l), e(n) } function d(t, r) { r = 0 === r ? 0 : r || 10; var n = e(t).toRgb(); return n.r = U(0, D(255, n.r - $(255 * -(r / 100)))), n.g = U(0, D(255, n.g - $(255 * -(r / 100)))), n.b = U(0, D(255, n.b - $(255 * -(r / 100)))), e(n) } function _(t, r) { r = 0 === r ? 0 : r || 10; var n = e(t).toHsl(); return n.l -= r / 100, n.l = R(n.l), e(n) } function p(t, r) { var n = e(t).toHsl(), a = (n.h + r) % 360; return n.h = 0 > a ? 360 + a : a, e(n) } function m(t) { var r = e(t).toHsl(); return r.h = (r.h + 180) % 360, e(r) } function v(t) { var r = e(t).toHsl(), n = r.h; return [e(t), e({ h: (n + 120) % 360, s: r.s, l: r.l }), e({ h: (n + 240) % 360, s: r.s, l: r.l })] } function y(t) { var r = e(t).toHsl(), n = r.h; return [e(t), e({ h: (n + 90) % 360, s: r.s, l: r.l }), e({ h: (n + 180) % 360, s: r.s, l: r.l }), e({ h: (n + 270) % 360, s: r.s, l: r.l })] } function A(t) { var r = e(t).toHsl(), n = r.h; return [e(t), e({ h: (n + 72) % 360, s: r.s, l: r.l }), e({ h: (n + 216) % 360, s: r.s, l: r.l })] } function x(t, r, n) { r = r || 6, n = n || 30; var a = e(t).toHsl(), i = 360 / n, s = [e(t)]; for (a.h = (a.h - (i * r >> 1) + 720) % 360; --r;)a.h = (a.h + i) % 360, s.push(e(a)); return s } function k(t, r) { r = r || 6; for (var n = e(t).toHsv(), a = n.h, i = n.s, s = n.v, o = [], f = 1 / r; r--;)o.push(e({ h: a, s: i, v: s })), s = (s + f) % 1; return o } function w(t) { var e = {}; for (var r in t) t.hasOwnProperty(r) && (e[t[r]] = r); return e } function S(t) { return t = parseFloat(t), (isNaN(t) || 0 > t || t > 1) && (t = 1), t } function H(e, r) { C(e) && (e = "100%"); var n = q(e); return e = D(r, U(0, parseFloat(e))), n && (e = parseInt(e * r, 10) / 100), t.abs(e - r) < 1e-6 ? 1 : e % r / parseFloat(r) } function R(t) { return D(1, U(0, t)) } function F(t) { return parseInt(t, 16) } function C(t) { return "string" == typeof t && -1 != t.indexOf(".") && 1 === parseFloat(t) } function q(t) { return "string" == typeof t && -1 != t.indexOf("%") } function M(t) { return 1 == t.length ? "0" + t : "" + t } function I(t) { return 1 >= t && (t = 100 * t + "%"), t } function L(e) { return t.round(255 * parseFloat(e)).toString(16) } function N(t) { return F(t) / 255 } function z(t) { return !!X.CSS_UNIT.exec(t) } function E(t) { t = t.replace(j, "").replace(O, "").toLowerCase(); var e = !1; if (G[t]) t = G[t], e = !0; else if ("transparent" == t) return { r: 0, g: 0, b: 0, a: 0, format: "name" }; var r; return (r = X.rgb.exec(t)) ? { r: r[1], g: r[2], b: r[3] } : (r = X.rgba.exec(t)) ? { r: r[1], g: r[2], b: r[3], a: r[4] } : (r = X.hsl.exec(t)) ? { h: r[1], s: r[2], l: r[3] } : (r = X.hsla.exec(t)) ? { h: r[1], s: r[2], l: r[3], a: r[4] } : (r = X.hsv.exec(t)) ? { h: r[1], s: r[2], v: r[3] } : (r = X.hsva.exec(t)) ? { h: r[1], s: r[2], v: r[3], a: r[4] } : (r = X.hex8.exec(t)) ? { r: F(r[1]), g: F(r[2]), b: F(r[3]), a: N(r[4]), format: e ? "name" : "hex8" } : (r = X.hex6.exec(t)) ? { r: F(r[1]), g: F(r[2]), b: F(r[3]), format: e ? "name" : "hex" } : (r = X.hex4.exec(t)) ? { r: F(r[1] + "" + r[1]), g: F(r[2] + "" + r[2]), b: F(r[3] + "" + r[3]), a: N(r[4] + "" + r[4]), format: e ? "name" : "hex8" } : (r = X.hex3.exec(t)) ? { r: F(r[1] + "" + r[1]), g: F(r[2] + "" + r[2]), b: F(r[3] + "" + r[3]), format: e ? "name" : "hex" } : !1 } function T(t) { var e, r; return t = t || { level: "AA", size: "small" }, e = (t.level || "AA").toUpperCase(), r = (t.size || "small").toLowerCase(), "AA" !== e && "AAA" !== e && (e = "AA"), "small" !== r && "large" !== r && (r = "small"), { level: e, size: r } } var j = /^\s+/, O = /\s+$/, P = 0, $ = t.round, D = t.min, U = t.max, B = t.random; e.prototype = { isDark: function () { return this.getBrightness() < 128 }, isLight: function () { return !this.isDark() }, isValid: function () { return this._ok }, getOriginalInput: function () { return this._originalInput }, getFormat: function () { return this._format }, getAlpha: function () { return this._a }, getBrightness: function () { var t = this.toRgb(); return (299 * t.r + 587 * t.g + 114 * t.b) / 1e3 }, getLuminance: function () { var e, r, n, a, i, s, o = this.toRgb(); return e = o.r / 255, r = o.g / 255, n = o.b / 255, a = .03928 >= e ? e / 12.92 : t.pow((e + .055) / 1.055, 2.4), i = .03928 >= r ? r / 12.92 : t.pow((r + .055) / 1.055, 2.4), s = .03928 >= n ? n / 12.92 : t.pow((n + .055) / 1.055, 2.4), .2126 * a + .7152 * i + .0722 * s }, setAlpha: function (t) { return this._a = S(t), this._roundA = $(100 * this._a) / 100, this }, toHsv: function () { var t = s(this._r, this._g, this._b); return { h: 360 * t.h, s: t.s, v: t.v, a: this._a } }, toHsvString: function () { var t = s(this._r, this._g, this._b), e = $(360 * t.h), r = $(100 * t.s), n = $(100 * t.v); return 1 == this._a ? "hsv(" + e + ", " + r + "%, " + n + "%)" : "hsva(" + e + ", " + r + "%, " + n + "%, " + this._roundA + ")" }, toHsl: function () { var t = a(this._r, this._g, this._b); return { h: 360 * t.h, s: t.s, l: t.l, a: this._a } }, toHslString: function () { var t = a(this._r, this._g, this._b), e = $(360 * t.h), r = $(100 * t.s), n = $(100 * t.l); return 1 == this._a ? "hsl(" + e + ", " + r + "%, " + n + "%)" : "hsla(" + e + ", " + r + "%, " + n + "%, " + this._roundA + ")" }, toHex: function (t) { return f(this._r, this._g, this._b, t) }, toHexString: function (t) { return "#" + this.toHex(t) }, toHex8: function (t) { return h(this._r, this._g, this._b, this._a, t) }, toHex8String: function (t) { return "#" + this.toHex8(t) }, toRgb: function () { return { r: $(this._r), g: $(this._g), b: $(this._b), a: this._a } }, toRgbString: function () { return 1 == this._a ? "rgb(" + $(this._r) + ", " + $(this._g) + ", " + $(this._b) + ")" : "rgba(" + $(this._r) + ", " + $(this._g) + ", " + $(this._b) + ", " + this._roundA + ")" }, toPercentageRgb: function () { return { r: $(100 * H(this._r, 255)) + "%", g: $(100 * H(this._g, 255)) + "%", b: $(100 * H(this._b, 255)) + "%", a: this._a } }, toPercentageRgbString: function () { return 1 == this._a ? "rgb(" + $(100 * H(this._r, 255)) + "%, " + $(100 * H(this._g, 255)) + "%, " + $(100 * H(this._b, 255)) + "%)" : "rgba(" + $(100 * H(this._r, 255)) + "%, " + $(100 * H(this._g, 255)) + "%, " + $(100 * H(this._b, 255)) + "%, " + this._roundA + ")" }, toName: function () { return 0 === this._a ? "transparent" : this._a < 1 ? !1 : V[f(this._r, this._g, this._b, !0)] || !1 }, toFilter: function (t) { var r = "#" + l(this._r, this._g, this._b, this._a), n = r, a = this._gradientType ? "GradientType = 1, " : ""; if (t) { var i = e(t); n = "#" + l(i._r, i._g, i._b, i._a) } return "progid:DXImageTransform.Microsoft.gradient(" + a + "startColorstr=" + r + ",endColorstr=" + n + ")" }, toString: function (t) { var e = !!t; t = t || this._format; var r = !1, n = this._a < 1 && this._a >= 0, a = !e && n && ("hex" === t || "hex6" === t || "hex3" === t || "hex4" === t || "hex8" === t || "name" === t); return a ? "name" === t && 0 === this._a ? this.toName() : this.toRgbString() : ("rgb" === t && (r = this.toRgbString()), "prgb" === t && (r = this.toPercentageRgbString()), "hex" !== t && "hex6" !== t || (r = this.toHexString()), "hex3" === t && (r = this.toHexString(!0)), "hex4" === t && (r = this.toHex8String(!0)), "hex8" === t && (r = this.toHex8String()), "name" === t && (r = this.toName()), "hsl" === t && (r = this.toHslString()), "hsv" === t && (r = this.toHsvString()), r || this.toHexString()) }, clone: function () { return e(this.toString()) }, _applyModification: function (t, e) { var r = t.apply(null, [this].concat([].slice.call(e))); return this._r = r._r, this._g = r._g, this._b = r._b, this.setAlpha(r._a), this }, lighten: function () { return this._applyModification(b, arguments) }, brighten: function () { return this._applyModification(d, arguments) }, darken: function () { return this._applyModification(_, arguments) }, desaturate: function () { return this._applyModification(u, arguments) }, saturate: function () { return this._applyModification(c, arguments) }, greyscale: function () { return this._applyModification(g, arguments) }, spin: function () { return this._applyModification(p, arguments) }, _applyCombination: function (t, e) { return t.apply(null, [this].concat([].slice.call(e))) }, analogous: function () { return this._applyCombination(x, arguments) }, complement: function () { return this._applyCombination(m, arguments) }, monochromatic: function () { return this._applyCombination(k, arguments) }, splitcomplement: function () { return this._applyCombination(A, arguments) }, triad: function () { return this._applyCombination(v, arguments) }, tetrad: function () { return this._applyCombination(y, arguments) } }, e.fromRatio = function (t, r) { if ("object" == typeof t) { var n = {}; for (var a in t) t.hasOwnProperty(a) && ("a" === a ? n[a] = t[a] : n[a] = I(t[a])); t = n } return e(t, r) }, e.equals = function (t, r) { return t && r ? e(t).toRgbString() == e(r).toRgbString() : !1 }, e.random = function () { return e.fromRatio({ r: B(), g: B(), b: B() }) }, e.mix = function (t, r, n) { n = 0 === n ? 0 : n || 50; var a = e(t).toRgb(), i = e(r).toRgb(), s = n / 100, o = { r: (i.r - a.r) * s + a.r, g: (i.g - a.g) * s + a.g, b: (i.b - a.b) * s + a.b, a: (i.a - a.a) * s + a.a }; return e(o) }, e.readability = function (r, n) { var a = e(r), i = e(n); return (t.max(a.getLuminance(), i.getLuminance()) + .05) / (t.min(a.getLuminance(), i.getLuminance()) + .05) }, e.isReadable = function (t, r, n) { var a, i, s = e.readability(t, r); switch (i = !1, a = T(n), a.level + a.size) { case "AAsmall": case "AAAlarge": i = s >= 4.5; break; case "AAlarge": i = s >= 3; break; case "AAAsmall": i = s >= 7 }return i }, e.mostReadable = function (t, r, n) { var a, i, s, o, f = null, h = 0; n = n || {}, i = n.includeFallbackColors, s = n.level, o = n.size; for (var l = 0; l < r.length; l++)a = e.readability(t, r[l]), a > h && (h = a, f = e(r[l])); return e.isReadable(t, f, { level: s, size: o }) || !i ? f : (n.includeFallbackColors = !1, e.mostReadable(t, ["#fff", "#000"], n)) }; var G = e.names = { aliceblue: "f0f8ff", antiquewhite: "faebd7", aqua: "0ff", aquamarine: "7fffd4", azure: "f0ffff", beige: "f5f5dc", bisque: "ffe4c4", black: "000", blanchedalmond: "ffebcd", blue: "00f", blueviolet: "8a2be2", brown: "a52a2a", burlywood: "deb887", burntsienna: "ea7e5d", cadetblue: "5f9ea0", chartreuse: "7fff00", chocolate: "d2691e", coral: "ff7f50", cornflowerblue: "6495ed", cornsilk: "fff8dc", crimson: "dc143c", cyan: "0ff", darkblue: "00008b", darkcyan: "008b8b", darkgoldenrod: "b8860b", darkgray: "a9a9a9", darkgreen: "006400", darkgrey: "a9a9a9", darkkhaki: "bdb76b", darkmagenta: "8b008b", darkolivegreen: "556b2f", darkorange: "ff8c00", darkorchid: "9932cc", darkred: "8b0000", darksalmon: "e9967a", darkseagreen: "8fbc8f", darkslateblue: "483d8b", darkslategray: "2f4f4f", darkslategrey: "2f4f4f", darkturquoise: "00ced1", darkviolet: "9400d3", deeppink: "ff1493", deepskyblue: "00bfff", dimgray: "696969", dimgrey: "696969", dodgerblue: "1e90ff", firebrick: "b22222", floralwhite: "fffaf0", forestgreen: "228b22", fuchsia: "f0f", gainsboro: "dcdcdc", ghostwhite: "f8f8ff", gold: "ffd700", goldenrod: "daa520", gray: "808080", green: "008000", greenyellow: "adff2f", grey: "808080", honeydew: "f0fff0", hotpink: "ff69b4", indianred: "cd5c5c", indigo: "4b0082", ivory: "fffff0", khaki: "f0e68c", lavender: "e6e6fa", lavenderblush: "fff0f5", lawngreen: "7cfc00", lemonchiffon: "fffacd", lightblue: "add8e6", lightcoral: "f08080", lightcyan: "e0ffff", lightgoldenrodyellow: "fafad2", lightgray: "d3d3d3", lightgreen: "90ee90", lightgrey: "d3d3d3", lightpink: "ffb6c1", lightsalmon: "ffa07a", lightseagreen: "20b2aa", lightskyblue: "87cefa", lightslategray: "789", lightslategrey: "789", lightsteelblue: "b0c4de", lightyellow: "ffffe0", lime: "0f0", limegreen: "32cd32", linen: "faf0e6", magenta: "f0f", maroon: "800000", mediumaquamarine: "66cdaa", mediumblue: "0000cd", mediumorchid: "ba55d3", mediumpurple: "9370db", mediumseagreen: "3cb371", mediumslateblue: "7b68ee", mediumspringgreen: "00fa9a", mediumturquoise: "48d1cc", mediumvioletred: "c71585", midnightblue: "191970", mintcream: "f5fffa", mistyrose: "ffe4e1", moccasin: "ffe4b5", navajowhite: "ffdead", navy: "000080", oldlace: "fdf5e6", olive: "808000", olivedrab: "6b8e23", orange: "ffa500", orangered: "ff4500", orchid: "da70d6", palegoldenrod: "eee8aa", palegreen: "98fb98", paleturquoise: "afeeee", palevioletred: "db7093", papayawhip: "ffefd5", peachpuff: "ffdab9", peru: "cd853f", pink: "ffc0cb", plum: "dda0dd", powderblue: "b0e0e6", purple: "800080", rebeccapurple: "663399", red: "f00", rosybrown: "bc8f8f", royalblue: "4169e1", saddlebrown: "8b4513", salmon: "fa8072", sandybrown: "f4a460", seagreen: "2e8b57", seashell: "fff5ee", sienna: "a0522d", silver: "c0c0c0", skyblue: "87ceeb", slateblue: "6a5acd", slategray: "708090", slategrey: "708090", snow: "fffafa", springgreen: "00ff7f", steelblue: "4682b4", tan: "d2b48c", teal: "008080", thistle: "d8bfd8", tomato: "ff6347", turquoise: "40e0d0", violet: "ee82ee", wheat: "f5deb3", white: "fff", whitesmoke: "f5f5f5", yellow: "ff0", yellowgreen: "9acd32" }, V = e.hexNames = w(G), X = function () { var t = "[-\\+]?\\d+%?", e = "[-\\+]?\\d*\\.\\d+%?", r = "(?:" + e + ")|(?:" + t + ")", n = "[\\s|\\(]+(" + r + ")[,|\\s]+(" + r + ")[,|\\s]+(" + r + ")\\s*\\)?", a = "[\\s|\\(]+(" + r + ")[,|\\s]+(" + r + ")[,|\\s]+(" + r + ")[,|\\s]+(" + r + ")\\s*\\)?"; return { CSS_UNIT: new RegExp(r), rgb: new RegExp("rgb" + n), rgba: new RegExp("rgba" + a), hsl: new RegExp("hsl" + n), hsla: new RegExp("hsla" + a), hsv: new RegExp("hsv" + n), hsva: new RegExp("hsva" + a), hex3: /^#?([0-9a-fA-F]{1})([0-9a-fA-F]{1})([0-9a-fA-F]{1})$/, hex6: /^#?([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})$/, hex4: /^#?([0-9a-fA-F]{1})([0-9a-fA-F]{1})([0-9a-fA-F]{1})([0-9a-fA-F]{1})$/, hex8: /^#?([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})$/ } }(); "undefined" != typeof module && module.exports ? module.exports = e : "function" == typeof define && define.amd ? define(function () { return e }) : window.tinycolor = e }(Math);

var SingularPlayer = (function () {
	var objectKeys = (typeof Object.keys !== 'undefined'),
		instances = {},
		readyMap = {},
		readyWindowStack = [],
		hostRegexp = new RegExp('^(http(?:s)?\://[^/]+)', 'im');

	function uuid() {
		var id = 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function (c) {
			var r = Math.random() * 16 | 0, v = c == 'x' ? r : (r & 0x3 | 0x8);
			return v.toString(16);
		});

		return id;
	}

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

	var defaultGradient = {
		type: 'solid',
		solidColor: {
			r: 204,
			g: 204,
			b: 204,
			a: 1.0
		},
		stops: [{ offset: 0.0, color: '#ff0000', opacity: 1 },
		{ offset: 0.5, color: '#00ff00', opacity: 1 },
		{ offset: 1.0, color: '#0000ff', opacity: 1 }],
		offset: 0.0,
		angle: 0,
		scale: 100.0,
		spreadMethod: 'pad',
		keepAspect: false,
		centerX: 50.0,
		centerY: 50.0,
		radius: 50.0,
		focalAngle: 0,
		focalDistance: 0
	}

	var isNumber = function (n) {
		return !isNaN(parseFloat(n)) && isFinite(n);
	}

	var checkDataType = function (model, payloadJSON) {

		var payLoadTypes = {};
		var outputPayload = {};
		var timeControlCollection = {};
		var buttonCollection = {};
		var setPayloadErrors = [];

		if (model) {
			let fields = model.fields;
			for (var id in fields) {
				payLoadTypes[fields[id].id] = fields[id].type;
			}
		}

		// check data type
		for (let field in payloadJSON) {

			if (isNumber(field)) {
				setPayloadErrors.push('Payload [' + field + '] cannot be a number.');
				continue;
			}

			if (payLoadTypes[field] == 'json' && typeof payloadJSON[field] == 'object') {
				setPayloadErrors.push('Payload [' + field + '] must be a JSON string.');
			} else if (payLoadTypes[field] == 'color') {
				var color = tinycolor(payloadJSON[field]);
				outputPayload[field] = color.toRgb();
			} else if (payLoadTypes[field] == 'gradient') {

				var tc = tinycolor(payloadJSON[field]);
				if (tc.isValid()) {
					var tmp = extend(true, defaultGradient);
					tmp.solidColor = tc.toRgb();
					outputPayload[field] = tmp;
				} else {
					if (typeof payloadJSON[field] == 'object')
						outputPayload[field] = extend(true, payloadJSON[field]);
					else
						outputPayload[field] = payloadJSON[field];
				}

			} else if (payLoadTypes[field] == 'checkbox') {
        if (!payloadJSON[field] || String(payloadJSON[field]).toLowerCase() === 'false') {
          outputPayload[field] = false;
        } else {
          outputPayload[field] = true;
        }
			} else if (payLoadTypes[field] == 'button') {
				buttonCollection[field] = true;
				if (payloadJSON[field].__singularButton && payloadJSON[field].ts !== undefined) // preserve original structure
					outputPayload[field] = extend(true, payloadJSON[field]);
				else if (payloadJSON[field])
					outputPayload[field] = 'execute';
			} else if (payLoadTypes[field] == 'timecontrol') {
				timeControlCollection[field] = true;
				var isObjectValid = false;
				if (payloadJSON[field].command && typeof payloadJSON[field].command == 'string' && (payloadJSON[field].command == 'play' || payloadJSON[field].command == 'pause' || payloadJSON[field].command == 'reset' || payloadJSON[field].command == 'start')) {
					isObjectValid = true;
				} else if (payloadJSON[field].UTC != undefined && isNumber(payloadJSON[field].UTC) &&
					payloadJSON[field].isRunning != undefined && typeof payloadJSON[field].isRunning == 'boolean' &&
					payloadJSON[field].value != undefined && isNumber(payloadJSON[field].value)) {
					isObjectValid = true;
				}

				outputPayload[field] = extend(true, payloadJSON[field]);

				if (!isObjectValid)
					setPayloadErrors.push('Payload [' + field + '] must be the following objects {command:"play"}, {command:"pause"}, {command:"reset"}, {command:"start"} or {UTC:number, isRunning:boolean, value:number}.');
			} else if (payLoadTypes[field] == 'location') {
				if (payloadJSON[field].text === undefined || payloadJSON[field].long === undefined || payloadJSON[field].lat === undefined) {
					setPayloadErrors.push('Payload [' + field + '] must be an object {text:"", long:number, lat:number}.');
				} else {
					outputPayload[field] = extend(true, payloadJSON[field]);
				}
			} else { // text, number, text area, audio, image, JSON
				if (typeof payloadJSON[field] == 'object') {
					if (payloadJSON[field] != null)
						setPayloadErrors.push('Payload [' + field + '] should not be an object.');
					outputPayload[field] = extend(true, payloadJSON[field]);
				}
				else {
					outputPayload[field] = payloadJSON[field];
				}
			}

		}

		// object must not be empty
		// value must not be null
		// or they will be removed from Firebase 
		for (let field in outputPayload) {
			if (typeof outputPayload[field] == 'object' && Object.keys(outputPayload[field]).length == 0) {
				setPayloadErrors.push('Payload [' + field + '] must not be an empty object or null.');
				delete outputPayload[field];
			}
		}

		return { errors: setPayloadErrors, payload: outputPayload, timeControlCollection, buttonCollection, payLoadTypes };
	}

	function SingularPlayer(domId, _apiKey) {
		return createInstance(domId, _apiKey);
	}

	function createInstance(domId, _apiKey) {
		var element = document.getElementById(domId);
		var apiKey = _apiKey;
		var instance = (function (element) {
			var isReady = false,
				isLoaded = false,
				instanceObj,
				embedHost,
				cmdQueue = [],
				cmdQueue2 = [],
				getters = {},
				events = {},
				ieHackEvent = [],
				composition_loaded_cb,
				compositionInfo = {},
				compositionInstances = {},
				sequencerInstances = {},
				subcompositionInstances = {},
				busyLoading = false,
				config,
				embedHost = getHostName(element.getAttribute('src')).toLowerCase(),
				buttonClickedTS = {},
				_config = {};

			function loadComposition(compositionId, cb) {
				addCommandQueue.apply(this, ['loadComposition', apiKey, compositionId, cb]);
			}

			function renderComposition(compositionObject, cb) {
				addCommandQueue.apply(this, ['renderComposition', compositionObject, cb]);
			}

			function renderAppOutput(appId, output, cb) {
				addCommandQueue.apply(this, ['renderAppOutput', apiKey, appId, output, cb]);
			}

			function videoCommand(cmd, time) {
				addCommandQueue.apply(this, ['videoCommand', cmd, time]);
			}

			function setAdaptationGlobals(data) {
				addCommandQueue.apply(this, ['setAdaptationGlobals', data]);
			}

			function setFrameNumber(frame) {
				addCommandQueue.apply(this, ['setFrameNumber', frame]);
			}

			function addCommandQueue(method) {
				if (!isReady) {
					if (!cmdQueue) {
						cmdQueue = [];
					}
					cmdQueue.push(arguments);
					return;
				}

				if (method == 'loadComposition') {
					if (!busyLoading) {
						busyLoading = true;
						composition_loaded_cb = arguments[3];
						sendMessage(element, embedHost, { event: 'loadComposition', params: { apiKey: arguments[1], id: arguments[2], config: config } });
					} else {
						dispatchEvent(events, 'error', { desc: 'Cannot process the command. The previous operation is still in progress.' });
					}
				} else if (method == 'renderComposition') {
					if (!busyLoading) {
						busyLoading = true;
						composition_loaded_cb = arguments[2];
						sendMessage(element, embedHost, { event: 'renderComposition', params: { compositionObject: arguments[1], config: config } });
					} else {
						dispatchEvent(events, 'error', { desc: 'Cannot process the command. The previous operation is still in progress.' });
					}
				} else if (method == 'renderAppOutput') {
					if (!busyLoading) {
						busyLoading = true;
						composition_loaded_cb = arguments[4];
						sendMessage(element, embedHost, { event: 'renderAppOutput', params: { apiKey: arguments[1], appId: arguments[2], output: arguments[3], config: config, noTrack: _config.noTrack } });
					} else {
						dispatchEvent(events, 'error', { desc: 'Cannot process the command. The previous operation is still in progress.' });
					}
				} else {
					// following commands will be executed after composition is loaded
					if (!isLoaded) {
						if (!cmdQueue2) {
							cmdQueue2 = [];
						}
						cmdQueue2.push(arguments);
					} else {
						if (method == 'jumpTo' || method == 'playTo' || method == 'seek') {
							sendMessage(element, embedHost, { event: method, params: { to: arguments[1] } });
						} else if (method == 'setControlNode' || method == 'control') {
							sendMessage(element, embedHost, { event: method, params: { obj: arguments[1] } });
						} else if (method == 'resize') {
							sendMessage(element, embedHost, { event: method, params: { width: arguments[1], height: arguments[2] } });
						} else if (method == 'destroy') {
							sendMessage(element, embedHost, { event: method });

							isReady = false;
							isLoaded = false;
							embedHost = '';
							cmdQueue = [];
							cmdQueue2 = [];
						} else if (method == 'comp_jumpTo') {
							var compId = compositionInfo.nameToId[arguments[1]];
							sendMessage(element, embedHost, { event: 'jumpTo', params: { compId: compId, to: arguments[2] } });
						} else if (method == 'comp_playTo') {
							var compId = compositionInfo.nameToId[arguments[1]];
							sendMessage(element, embedHost, { event: 'playTo', params: { compId: compId, to: arguments[2] } });
						} else if (method == 'comp_seek') {
							var compId = compositionInfo.nameToId[arguments[1]];
							sendMessage(element, embedHost, { event: 'seek', params: { compId: compId, to: arguments[2] } });
						} else if (method == 'sequencer_command') {
							sendMessage(element, embedHost, { event: method, params: { cmd: arguments[1], compId: arguments[2], seqId: arguments[3], params: arguments[4] } });
						} else if (method == 'comp_command') {// added in v0.1.0
							if (arguments[1] == 'setPayload')
								sendMessage(element, embedHost, { event: 'comp_command', params: { command: arguments[1], compId: arguments[2], payload: arguments[3] } });
							else if (arguments[1] == 'message')
								sendMessage(element, embedHost, { event: 'comp_command', params: { command: arguments[1], compId: arguments[2], message: arguments[3] } });
							else
								sendMessage(element, embedHost, { event: 'comp_command', params: { command: arguments[1], compId: arguments[2], to: arguments[3] } });
						} else if (method == 'videoCommand') {
							sendMessage(element, embedHost, { event: 'videoCommand', params: { command: arguments[1], time: arguments[2] } });
						} else if (method == 'setAdaptationGlobals') {
							compositionInfo.adaptationGlobals = extend(true, compositionInfo.adaptationGlobals, arguments[1]);
							sendMessage(element, embedHost, { event: 'setAdaptationGlobals', params: { data: compositionInfo.adaptationGlobals } });
							dispatchEvent(events, 'adaptation_globals_changed', compositionInfo.adaptationGlobals);
						} else if (method == 'setFrameNumber') {
							sendMessage(element, embedHost, { event: 'setFrameNumber', params: { frame: arguments[1] } });
						}
					}
				}
			}

			function execCommandQueue() {
				if (cmdQueue) {
					while (cmdQueue.length) {
						addCommandQueue.apply(this, cmdQueue.shift());
					}
					cmdQueue = null;
				}
			}

			function execCommandQueue2() {
				if (cmdQueue2) {
					while (cmdQueue2.length) {
						addCommandQueue.apply(this, cmdQueue2.shift());
					}
					cmdQueue2 = null;
				}
			}

			function ready() {
				isReady = true;
				//sendMessage(element, embedHost, {event: 'apihandshake', args: []});
				execCommandQueue();
			}

			function loaded() {
				isLoaded = true;
				execCommandQueue2();
			}

			return instanceObj = {
				host: embedHost,
				loadComposition: loadComposition,
				renderComposition: renderComposition,
				renderAppOutput: renderAppOutput,
				videoCommand: videoCommand,
				setAdaptationGlobals: setAdaptationGlobals,
				setFrameNumber: setFrameNumber,

				setConfig: function (_config) {
					config = _config;
				},

				getConfig: function () {
					return extend(true, config);
				},

				noTrack: function() {
					_config.noTrack = true;
				},

				addListener: function (event, callback) {

					const allEvents = ['message', 'state_changed', 'payload_changed', 'datanode_payload_changed', 'error', 'adaptation_globals_changed', 'composition_script_event', 'download_start', "download_complete"];

					if (allEvents.indexOf(event) != -1) {
						if (!events[event]) {
							events[event] = [];
						}
						events[event].push(callback);
					} else {
						throw new Error(`Listener event type '${event}' not supported.`);
					}
				},

				removeListener: function (event, callback) {
					if (callback) {
						for (var i = 0, el = events[event].length; i < el; i++) {
							if (events[event][i] === callback) {
								events[event].splice(i, 1);
							}
						}
					} else {
						events[event] = null;
					}
				},

				removeAllListener: function () {
					for (var key in events) {
						if (events.hasOwnProperty(key)) {
							events[key] = null;
						}
					}
				},

				onmessage: function (e) {
					if (e.origin.toLowerCase() == embedHost) {
						if (e.data.event == '_singular_ready') {
							ready();
						} else if (e.data.event == '_singular_composition_ready') {
							busyLoading = false;
							compositionInfo = e.data.params;

							if (compositionInfo && compositionInfo.dataSources) {
								for (var compId in compositionInfo.dataSources) {
									if (compositionInfo.dataSources[compId] &&
										compositionInfo.dataSources[compId].composition &&
										compositionInfo.dataSources[compId].composition.controlNode &&
										compositionInfo.dataSources[compId].composition.controlNode.model) {
										var fields = compositionInfo.dataSources[compId].composition.controlNode.model.fields;
										var dstPayload = compositionInfo.dataSources[compId].composition.controlNode.payload;
										for (var id in fields) {
											if (fields[id].type == 'button') {
												if (!buttonClickedTS[compId])
													buttonClickedTS[compId] = {}

												buttonClickedTS[compId][fields[id].id] = dstPayload[fields[id].id].ts;
											}
										}
									}
								}
							}

							loaded();
							if (composition_loaded_cb) {
								composition_loaded_cb({ success: true });
							}
						} else if (e.data.event == '_singular_composition_state_changed') {
							if (compositionInfo.idToName) {
								e.data.params.compositionName = compositionInfo.idToName[e.data.params.compositionId];
								if (e.data.params.compositionName == undefined)
									e.data.params.compositionName = 'Root';

								//console.log(e.data.params.compositionId, compositionInfo.compositionStates[e.data.params.compositionId], e.data.params.state);

								if (compositionInfo.compositionStates[e.data.params.compositionId] != e.data.params.state) {
									dispatchEvent(events, 'state_changed', e.data.params);
									compositionInfo.compositionStates[e.data.params.compositionId] = e.data.params.state;
								}

							}
						} else if (e.data.event == '_singular_composition_load_fail') {
							busyLoading = false;
							if (composition_loaded_cb) {
								composition_loaded_cb({ success: false });
							}
						} else if (e.data.event == '_singular_error') {
							dispatchEvent(events, 'error', e.data.params);
						} else if (e.data.event == '_singular_app_output_changed') {
							dispatchEvent(events, 'app_output_changed', e.data.params);
						} else if (e.data.event == '_singular_message') {
							dispatchEvent(events, 'message', e.data.params);
						} else if (e.data.event == '_singular_composition_payload_changed') {
							var compId = e.data.params.compId;
							var payload = e.data.params.payload;
							var buttonIds = {};
							if (compositionInfo && compositionInfo.dataSources && compositionInfo.dataSources[compId] && compositionInfo.dataSources[compId].composition) {
								var dstPayload = compositionInfo.dataSources[compId].composition.controlNode.payload;
								for (var key in payload) {
									if (dstPayload[key] !== undefined)
										dstPayload[key] = payload[key];
								}

								var fields = compositionInfo.dataSources[compId].composition.controlNode.model.fields
								for (var id in fields) {
									if (fields[id].type == 'button')
										buttonIds[fields[id].id] = true;
								}
							}

							// generate comp name
							if (compositionInfo && compositionInfo.subcompositionNames) {
								for (var k1 in compositionInfo.subcompositionNames) {
									for (var k2 in compositionInfo.subcompositionNames[k1]) {
										if (k2 == e.data.params.compId) {
											e.data.params.compositionName = e.data.params.compName = compositionInfo.subcompositionNames[k1][k2];
										}
									}
								}
							}

							if (compId == compositionInfo.compositionId) {
								e.data.params.compositionName = e.data.params.compName = 'Root';
							}

							dispatchEvent(events, 'payload_changed', e.data.params);

							// generates button_clicked
							for (var id in e.data.params.payload) {
								if (buttonIds[id]) {

									if (!buttonClickedTS[compId])
										buttonClickedTS[compId] = {}

									var btnParams = extend(true, e.data.params);
									var btnTs = btnParams.payload[id].ts

									if (buttonClickedTS[compId][id] != btnParams.payload[id].ts) {

										delete btnParams.payload;
										btnParams.button = id;

										dispatchEvent(events, 'button_clicked', btnParams);
									}

									buttonClickedTS[compId][id] = btnTs;

								}
							}


						} else if (e.data.event == '_singular_composition_datanode_changed') {
							var dataNode = e.data.params.dataNode;
							if (!compositionInfo.dataNodes)
								compositionInfo.dataNodes = {};

							compositionInfo.dataNodes[dataNode.id] = dataNode;
							compositionInfo.dataNodes[dataNode.id].url = this.host + '/apiv1/datanodes/' + dataNode.refId;
							if (e.data.dispatch === true)
								dispatchEvent(events, 'datanode_payload_changed', { payload: dataNode.payload, id: dataNode.id, name: dataNode.name });

						} else if (e.data.event == '_singular_setAdaptationGlobals') { // set from composition script editor to update data in sdk
							compositionInfo.adaptationGlobals = extend(true, compositionInfo.adaptationGlobals, e.data.params.adaptationGlobals);
							dispatchEvent(events, 'adaptation_globals_changed', compositionInfo.adaptationGlobals);
						} else if (e.data.event == '_singular_composition_download_start') {
							dispatchEvent(events, 'download_start', e.data.params);
						} else if (e.data.event == '_singular_composition_download_complete') {
							dispatchEvent(events, 'download_complete', e.data.params);
						} else if (e.data.event == '_singular_composition_timeline_event') {
							if (compositionInfo.idToName) {
								e.data.params.compositionName = compositionInfo.idToName[e.data.params.compositionId];

								if (e.data.params.compositionId == compositionInfo.compositionId)
									e.data.params.compositionName = 'Root';

								dispatchEvent(events, 'timeline_event', e.data.params);
							}
						} else if (e.data.event == '_singular_composition_script_event') {
							dispatchEvent(events, 'composition_script_event', e.data.params);
						}
					}
				},

				getCompositionInfo: function () {
					return compositionInfo;
				},

				getAdaptationGlobals: function () {
					return compositionInfo.adaptationGlobals;
				},

				getMainComposition: function () {
					if (compositionInfo && compositionInfo.compositionId)
						return this.getCompositionById(compositionInfo.compositionId);
					else
						return undefined;
				},

				getSequencer: function () {
					var newSequencerInstance;
					var seqId = mainId = compositionInfo.compositionId;

					if (sequencerInstances[mainId]) return sequencerInstances[mainId];

					var newSequencerInstance = (function () {

						var seq_start = function () {
							addCommandQueue.apply(this, ['sequencer_command', 'start', mainId, seqId, {}]);
						}

						var seq_stop = function () {
							addCommandQueue.apply(this, ['sequencer_command', 'stop', mainId, seqId, {}]);
						}

						var seq_seek = function (time) {
							addCommandQueue.apply(this, ['sequencer_command', 'seek', mainId, seqId, { time: time }]);
						}

						var seq_play = function (time) {
							addCommandQueue.apply(this, ['sequencer_command', 'play', mainId, seqId, {}]);
						}

						var seq_setDuration = function (duration) {
							addCommandQueue.apply(this, ['sequencer_command', 'setDuration', mainId, seqId, { duration: duration }]);
						}

						var seq_setPayload = function (payload) {
							if (payload.constructor === Object) { // new sequencer function in the graphics SDK

								for (var payloadCompId in payload) {
									addCommandQueue.apply(this, ['comp_command', 'jumpTo', payloadCompId, 'Out']);
								}
								addCommandQueue.apply(this, ['sequencer_command', 'setPayload', mainId, seqId, { payload: payload }]);
							}
						}

						return {
							start: seq_start,
							stop: seq_stop,
							seek: seq_seek,
							play: seq_play,
							setDuration: seq_setDuration,
							setPayload: seq_setPayload,
							seqId: seqId,
						}
					})();

					sequencerInstances[mainId] = newSequencerInstance;
					return sequencerInstances[mainId];
				},

				getCompositionById: function (compId) {
					if (compId == undefined) return this.getMainComposition();

					if (compositionInfo && !compositionInfo.subcompositionNames[compId])
						return undefined;

					var _this = this;
					if (compositionInstances[compId]) return compositionInstances[compId];

					var newCompositionInstance = function () {
						var id = compId;

						var comp_jumpTo = function (to) {
							addCommandQueue.apply(this, ['comp_command', 'jumpTo', compId, to]);
						}

						var comp_playTo = function (to) {
							addCommandQueue.apply(this, ['comp_command', 'playTo', compId, to]);
						}

						var comp_seek = function (to) {
							addCommandQueue.apply(this, ['comp_command', 'seek', compId, to]);
						}

						var comp_setPayload = function (_payload) {

							let setPayloadErrors = [];
							// the payload in compositionInfo will be updated by _singular_composition_payload_changed
							// but if setPayload is called, it will set to local
							if (compositionInfo && compositionInfo.dataSources && compositionInfo.dataSources[compId] && compositionInfo.dataSources[compId].composition) {
								var { errors, payload, timeControlCollection, buttonCollection, payLoadTypes } = checkDataType(compositionInfo.dataSources[compId].composition.controlNode.model, _payload);
								var dstPayload = compositionInfo.dataSources[compId].composition.controlNode.payload;
								for (var key in payload) {
									if (payLoadTypes[key] !== undefined) {

										var doUpdate = true;
										if (buttonCollection[key] && typeof payload[key] == 'string' && payload[key].toLowerCase() == 'execute') {
											payload[key] = { "__singularButton": true, "ts": new Date().valueOf() };
										} else if (timeControlCollection[key]) {

											var ServerDateDotNow = Date.now(); // for Hubert
											var tcCommand = payload[key].command;
											// the current payload of the time object
											const pValue = dstPayload[key];

											if (tcCommand == 'play') {
												// if the timer is already playing we don't have to do anything
												if (!pValue.isRunning) {
													payload[key] = {
														UTC: ServerDateDotNow,
														isRunning: true,
														value: (pValue.value ? pValue.value : 0)
													};
												} else {
													doUpdate = false;
												}
											} else if (tcCommand == 'pause') {
												// only if the time is running
												if (pValue.isRunning) {
													const serverUTC = ServerDateDotNow;

													// use default values of values from te payload
													let value = 0, UTC = serverUTC;
													if (pValue) {
														value = (pValue.value ? pValue.value : 0);
														UTC = (pValue.UTC ? pValue.UTC : UTC);
													}

													// increase the value by the time spent running
													value += serverUTC - UTC;

													// stop the clock, update the UTC timestamp and set the new value
													payload[key] = {
														UTC: serverUTC,
														isRunning: false,
														value: value
													};
												}
											} else if (tcCommand == 'reset') {
												payload[key] = {
													UTC: ServerDateDotNow,
													isRunning: false,
													value: 0
												};
											}
										}

										if (doUpdate)
											dstPayload[key] = payload[key];
									}
								}
							}

							setPayloadErrors = errors;

							addCommandQueue.apply(this, ['comp_command', 'setPayload', compId, payload]);

							if (setPayloadErrors && setPayloadErrors.length > 0) {
								let errorMsg = '';

								setPayloadErrors.forEach(msg => {
									errorMsg += msg + ' ';
								});

								console.warn(errorMsg);
							}
						}

						var comp_sendMessage = function (message) {
							addCommandQueue.apply(this, ['comp_command', 'message', compId, message]);
						}

						var comp_getSubcompositionById = function (subCompId) {
							return _this.getCompositionById(subCompId);
						}

						var comp_getState = function () {
							var state = compositionInfo.compositionStates[compId];
							if (state == 'In') return 'In';
							else if (state == 'Out1' || state == 'Out2') return 'Out';
						}

						var comp_listSubcompositions = function () {
							var list = [];
							if (compositionInfo && compositionInfo.subcompositionNames[compId]) {
								for (var sKey in compositionInfo.subcompositionNames[compId]) {
									list.push({ id: sKey, name: compositionInfo.subcompositionNames[compId][sKey] });
								}
							}
							return list;
						}

						var comp_getDataNodes = function () {
							var ret = {}
							if (compositionInfo && compositionInfo.dataSources && compositionInfo.dataSources[compId] && compositionInfo.dataSources[compId].composition) {
								var dataNodes = compositionInfo.dataSources[compId].composition.dataNodes;
								for (var d in dataNodes) {
									var id = dataNodes[d].dataNode;
									if (compositionInfo.dataNodes && compositionInfo.dataNodes[id])
										ret[id] = compositionInfo.dataNodes[id];
								}
							}

							return ret;
						}

						var comp_getModel = function () {
							if (compositionInfo && compositionInfo.dataSources && compositionInfo.dataSources[compId] && compositionInfo.dataSources[compId].composition) {
								var fields = compositionInfo.dataSources[compId].composition.controlNode.model.fields;
								var fieldArray = []
								for (var f in fields) {
									fieldArray.push(fields[f])
								}
								return fieldArray;
							}

							return [];
						}

						var comp_getPayload = function () {
							if (compositionInfo && compositionInfo.dataSources && compositionInfo.dataSources[compId] && compositionInfo.dataSources[compId].composition) {
								var payload = compositionInfo.dataSources[compId].composition.controlNode.payload;
								var payloadArray = []
								for (var p in payload) {
									payloadArray.push({ key: p, value: payload[p] });
								}
								return payloadArray;
							}

							return [];
						}

						var comp_getPayload2 = function () {
							if (compositionInfo && compositionInfo.dataSources && compositionInfo.dataSources[compId] && compositionInfo.dataSources[compId].composition) {
								var payload = extend(compositionInfo.dataSources[compId].composition.controlNode.payload);
								return payload;
							}

							return payload;
						}

						var comp_getControlNode = function () {
							if (compositionInfo && compositionInfo.dataSources && compositionInfo.dataSources[compId] && compositionInfo.dataSources[compId].composition)
								return compositionInfo.dataSources[compId].composition.controlNode;

							return undefined;
						}

						var comp_getLogicLayer = function () {
							if (compositionInfo && compositionInfo.compositionProps && compositionInfo.compositionProps.logicLayers && compositionInfo.compositionProps.logicLayers)
								return compositionInfo.compositionProps.logicLayers[compId];

							return undefined;
						}

						var _findComposition = function (parentId, searchPath) {
							var subNames = compositionInfo.subcompositionNames;
							var targetName = searchPath[searchPath.length - 1];
							var targetKeys = [];


							var fc = function (id, index) {
								for (var key in subNames[id]) {
									if (subNames[id][key] == searchPath[index]) {
										if (index < searchPath.length - 1) {
											fc(key, index + 1);
										} else {
											targetKeys.push(key);
										}
									}
								}
							}

							fc(parentId, 0);

							var findParentId = function (childId) {
								for (var key in subNames) {
									if (subNames[key][childId]) return key;
								}
							}

							var results = [];

							targetKeys.forEach(key => {
								var id = key;
								var idPath = []; // id path from parent to child
								while (id != parentId && id != undefined) {
									idPath.unshift(id);
									id = findParentId(id);
								}

								if (idPath.length > 0) {
									var comp = _this.getCompositionById(idPath[0]);
									for (var i = 1; i < idPath.length; i++) {
										comp = comp.getCompositionById(idPath[i]);
									}
									results.push(comp);
								}

							})

							return results;
						}

						var findComposition = function () {
							var startingCompName = arguments[0];
							var subNames = compositionInfo.subcompositionNames;
							var startingParentIds = [];

							// find parent id of the childs we are looking for
							var findParentIdByChildName = function (id, name) {
								for (var childKey in subNames[id]) {
									if (subNames[id][childKey] == name) {
										startingParentIds.push(id);
									}
									findParentIdByChildName(childKey, name);
								}
							}

							findParentIdByChildName(compId, startingCompName);

							// _findComposition takes id of the parent to start looking down for the children
							var results = [];
							var args = Array.prototype.slice.call(arguments);
							startingParentIds.forEach(id => {
								results = results.concat(_findComposition(id, args));
							})
							return results;
						}

						var _getSubcomps = function (subcompositionNames) {
							var result = [];
							var _getSubcomp = function (id) {
								for (var k in subcompositionNames[id]) {
									result.push(k);
									_getSubcomp(k);
								}
							}
							_getSubcomp(compId);
							return result;
						}

						return {
							id: compId,
							jumpTo: comp_jumpTo,
							playTo: comp_playTo,
							seek: comp_seek,
							setPayload: comp_setPayload,
							sendMessage: comp_sendMessage,
							getSubcompositionById: comp_getSubcompositionById,
							getCompositionById: comp_getSubcompositionById, // in player sdk, comp and sub comp are not internally different.
							listSubcompositions: comp_listSubcompositions,
							getDataNodes: comp_getDataNodes,
							getModel: comp_getModel,
							getPayload: comp_getPayload,
							getPayload2: comp_getPayload2,
							getControlNode: comp_getControlNode,
							getLogicLayer: comp_getLogicLayer,
							getState: comp_getState,
							find: findComposition,
						};
					}();

					compositionInstances[compId] = newCompositionInstance;
					return compositionInstances[compId];
				},
			};
		}(element));

		if (!element) {
			console.log('Target dom ID: ' + domId + ' not found');
			return;
		}

		instance.id = element.id;
		instances[element.id] = instance;

		for (var readyIndex in readyWindowStack) {
			if (element.contentWindow === readyWindowStack[readyIndex].source) {
				instances[element.id].onmessage(readyWindowStack[readyIndex]);
			}
		}

		return instance;
	}

	function getIframe(iframe) {
		if (typeof iframe === "string") {
			iframe = document.getElementById(iframe);
		}
		return iframe;
	}

	function dispatchEvent(events, event, data) {
		for (var cb in events[event]) {
			if (events[event].hasOwnProperty(cb)) {
				events[event][cb].call(window, event, data);
			}
		}
	}

	function callGetter(getters, event, data) {
		if (!getters[event]) {
			return;
		}

		// keep reference to array only here
		var items = getters[event];

		// then delete the original array
		getters[event] = null;
		delete getters[event];

		items.forEach(function (item) {
			item.call(window, data);
		});
	}

	function onMessage(e) {
		var ins, doc;
		if (e.data.event == '_singular_ready') {
			readyWindowStack.push(e);
		}

		for (ins in instances) {
			if (instances.hasOwnProperty(ins) && instances[ins]) {
				doc = document.getElementById(ins);

				if (doc && doc.contentWindow) {
					// fromScriptEditor tells you that this message is only from the script editor to use in editor env and you should not ignore it
					if (doc.contentWindow === e.source || e.data.fromScriptEditor) {
						instances[ins].onmessage(e);
					}
				} else if (typeof e.source === "string" && e.source == ins) {
					instances[ins].onmessage(e);
				}
			}
		}
	}

	function sendMessage(element, host, data) {
		// prefix with '_singular_'
		data.event = '_singular_' + data.event;

		if (element.contentWindow)
			element.contentWindow.postMessage(JSON.stringify(data), host)
	}

	function getHostName(url) {
		if (url.indexOf('http') < 0) {
			url = location.href;
		}
		return url.match(hostRegexp)[1].toString();
	}

	function makeArray(smtg) {
		return Array.prototype.slice.call(smtg, 0);
	}

	function addDomEvent(target, event, cb) {
		if (target.addEventListener) {
			target.addEventListener(event, cb, false);
		} else {
			target.attachEvent('on' + event, cb);
		}
	}

	addDomEvent(window, 'message', onMessage);

	if (typeof define === 'function' && define.amd) {
		define('singularplayer-api', [], function () {
			return SingularPlayer;
		});
	}

	return (window.SingularPlayer = SingularPlayer);

})();
