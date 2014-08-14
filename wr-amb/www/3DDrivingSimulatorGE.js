/**
 *	3D Driving Simulator on Google Earth
 *	Copyright (C) 2012-2013 Katsuomi Kobayashi http://www.geoquake.com/
 *
 *	This software is powered by
 *
 *	Google Earth API
 *	https://developers.google.com/earth/
 *	Haxe
 *	http://haxe.org/
 *	Box2D for Haxe
 *	http://code.google.com/p/box2d-haxelib/
 */

/*
* Box2D
* Copyright (c) 2006-2007 Erin Catto http://www.gphysics.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

(function () {
	"use strict";
	function t(e, t) {
		function n() {}
		n.prototype = e;
		var r = new n;
		for (var i in t) r[i] = t[i];
		return r
	}
	function E(e, t) {
		var n = function () {
			return n.method.apply(n.scope, arguments)
		};
		return n.scope = e,
		n.method = t,
		n
	}
	function L(e, t) {
		var n = typeof window != "undefined" ? window: exports,
		r = t.split(".");
		for (var i = 0; i < r.length - 1; ++i) {
			var s = r[i];
			typeof n[s] == "undefined" && (n[s] = {}),
			n = n[s]
		}
		n[r[r.length - 1]] = e
	}
	var e = function () {
		return y.Boot.__string_rec(this, "")
	},
	n = function () {
		this.wheelAngle = 0,
		this.isRightHandDrive = !1,
		this.viewPointIndex = 1,
		this.virtualInput = new p,
		this.viewPoints = [new h(1.2, 0, 90), new h(1.7, -6, 83), new h(3.5, -10, 79), new h(20, -20, 45), new h(50, 8, 0)]
	};
	n.__name__ = !0,
	n.prototype = {
		setActive: function (e) {
			this.virtualInput.setActive(e)
		},
		getVehicleHeading: function () {
			return this.vehicle.getHeading()
		},
		getLatLngAlt: function (e, t, n) {
			var r = new v.geoquake.geometry.Vector3(e, t, n),
			i = new v.geoquake.geometry.Matrix3x3;
			i.setRotationY( - this.vehicleRoll.position),
			r.multiply(i),
			i.setRotationX( - this.vehicleTilt.position),
			r.multiply(i);
			var s = -Math.atan2(r.y, r.x) + 1.570796326794897,
			u = v.geoquake.MyMath.lengthVector(r.x, r.y),
			a = this.vehiclePosition.createOffset(this.getVehicleHeading() + s, u),
			f = this.getAltitude(this.vehiclePosition) + r.z;
			return new o(a.lat, a.lng, f)
		},
		updateSpeedMeter: function () {
			this.speedKmhSpriteNumber.setValue(Math.round(this.vehicle.getSpeedKmh())),
			this.speedMphSpriteNumber.setValue(Math.round(this.vehicle.getSpeedMph()));
			var e = this.vehicle.getGVector(),
			t = -64 / 9.8;
			this.gravityMarker.setPosition(664 - e.x * t, 258 - e.y * t, 0, 0)
		},
		updateLights: function (e) {
			var t = this,
			n = this.viewPoints[this.viewPointIndex],
			r = new v.geoquake.geometry.Vector3(0, n.offset, n.altitude),
			i = new v.geoquake.geometry.Matrix3x3;
			i.setRotationZ(this.getVehicleHeading() - this.cameraHeading),
			r.multiply(i);
			var s = .4,
			o = .425,
			u = new v.geoquake.geometry.Vector2( - this.vehicleSpec.bodyWidth * s, this.vehicleSpec.bodyLength * o),
			a = new v.geoquake.geometry.Vector2(this.vehicleSpec.bodyWidth * s, this.vehicleSpec.bodyLength * o),
			f = new v.geoquake.geometry.Vector2( - this.vehicleSpec.bodyWidth * s, -this.vehicleSpec.bodyLength * o),
			l = new v.geoquake.geometry.Vector2(this.vehicleSpec.bodyWidth * s, -this.vehicleSpec.bodyLength * o),
			c = new v.geoquake.geometry.LineSegment2(u, a),
			h = new v.geoquake.geometry.LineSegment2(f, l),
			p = new v.geoquake.geometry.LineSegment2(u, f),
			d = new v.geoquake.geometry.LineSegment2(a, l),
			m = new v.geoquake.geometry.Vector2(r.x, r.y),
			g = function (e, n, i, s, o) {
				if (t.viewPointIndex != 4) {
					var u = new v.geoquake.geometry.Vector2(n, i),
					a = new v.geoquake.geometry.LineSegment2(m, u);
					if (v.geoquake.geometry.LineSegment2.intersect(a, c) || v.geoquake.geometry.LineSegment2.intersect(a, h) || v.geoquake.geometry.LineSegment2.intersect(a, p) || v.geoquake.geometry.LineSegment2.intersect(a, d)) {
						e.setOpacity(0);
						return
					}
				}
				var f = t.getLatLngAlt(n, i, s),
				l = new v.geoquake.geometry.Vector3(n, i, s),
				g = v.geoquake.geometry.Vector3.distance(l, r),
				y = 500 / g;
				e.setOpacity(o);
				if (o > 0) {
					var b = t.ge.getView().project(f.lat, f.lng, f.alt, t.ge.ALTITUDE_ABSOLUTE);
					e.setPosition(b.getX(), 360 - b.getY(), .5, .5),
					e.setSize(y, y)
				}
			},
			y = this.vehicle.getBrakeLightsBrightness(),
			b = this.vehicle.getReverseLightsBrightness(),
			w = this.vehicle.getTurnSignalsLeftBrightness(),
			E = this.vehicle.getTurnSignalsRightBrightness(),
			S = -2.187,
			x = .775,
			T = -0.58,
			N = -0.45,
			C = -0.705,
			k = -0.8;
			g(this.brakeLightL, T, S, x, y),
			g(this.brakeLightR, -T, S, x, y),
			g(this.reverseLightL, N, S, x, b),
			g(this.reverseLightR, -N, S, x, b),
			g(this.turnSignalLightFL, k, 1.73, .652, w),
			g(this.turnSignalLightFR, -k, 1.73, .652, E),
			g(this.turnSignalLightRL, C, S, x, w),
			g(this.turnSignalLightRR, -C, S, x, E),
			this.viewPointIndex == 0 ? (this.driverTurnSignalL.setOpacity(w), this.driverTurnSignalR.setOpacity(E)) : (this.driverTurnSignalL.setOpacity(0), this.driverTurnSignalR.setOpacity(0))
		},
		updateCamera: function (e) {
			var t = 1 - Math.exp( - 6 * e),
			n = v.geoquake.Angle.wrap(this.getVehicleHeading());
			this.cameraHeadingBase += v.geoquake.Angle.wrap(n - this.cameraHeadingBase) * t,
			this.cameraHeadingBase = v.geoquake.Angle.wrap(this.cameraHeadingBase);
			var r = this.virtualInput.getViewY(),
			i = this.virtualInput.getViewX(),
			o = r < -0.5,
			u = this.viewPoints[this.viewPointIndex],
			a = 0,
			f = 0;
			r > 0 && (f = .4363323129985825 * r),
			a = .872664625997165 * i;
			var l = -this.vehicleTilt.position,
			c = this.vehicleRoll.position;
			o && (a = 3.141592653589794, l = -l, c = -c),
			l = u.tilt * .0174532925199433 + l + f;
			var h, p, d, m, g, y = this;
			switch (y.viewPointIndex) {
			case 0:
				this.cameraHeading = v.geoquake.Angle.wrap(this.getVehicleHeading()) + a,
				m = l,
				g = c;
				var b;
				o ? (b = this.getLatLngAlt(.35 * (this.isRightHandDrive ? 1 : -1) + i * .3, 0, 1.3), m -= .17453292519943298) : b = this.getLatLngAlt(.35 * (this.isRightHandDrive ? 1 : -1), 0, 1.2),
				h = b.lat,
				p = b.lng,
				d = b.alt - this.getAltitude(new s(h, p));
				break;
			case 3:
				this.cameraHeading = 0,
				m = u.tilt * .0174532925199433,
				g = 0;
				var w = v.geoquake.MyMath.square(this.vehicle.getSpeedKmh());
				u.altitude = v.geoquake.MyMath.linearMap(w, 0, 1e4, 20, 60),
				u.offset = v.geoquake.MyMath.linearMap(w, 0, 1e4, -20, -60);
				var E = this.vehiclePosition.createOffset(this.cameraHeading, u.offset),
				S = v.geoquake.MyMath.square(this.vehicle.getSpeedKmh()),
				x = v.geoquake.MyMath.linearMap(S, 0, 1e4, 2, 10),
				b = E.createOffset(this.getVehicleHeading(), x);
				h = b.lat,
				p = b.lng,
				d = this.getAltitude(this.vehiclePosition) + u.altitude - this.getAltitude(new s(h, p));
				break;
			case 4:
				this.cameraHeading = this.cameraHeadingBase,
				m = u.tilt * .0174532925199433,
				g = 0;
				var w = v.geoquake.MyMath.square(this.vehicle.getSpeedKmh());
				u.altitude = v.geoquake.MyMath.linearMap(w, 0, 1e4, 50, 100),
				u.offset = v.geoquake.MyMath.linearMap(w, 0, 1e4, 5, 18);
				var T = this.vehiclePosition.createOffset(this.cameraHeading, u.offset);
				h = T.lat,
				p = T.lng,
				d = this.getAltitude(this.vehiclePosition) + u.altitude - this.getAltitude(new s(h, p));
				break;
			default:
				this.cameraHeading = this.cameraHeadingBase + a,
				m = l,
				g = c;
				var N = new v.geoquake.geometry.Vector3(0, u.offset, 0);
				N.rotateZ(this.getVehicleHeading() - this.cameraHeading);
				var T = this.getLatLngAlt(N.x, N.y, u.altitude);
				h = T.lat,
				p = T.lng,
				d = T.alt - this.getAltitude(new s(h, p))
			}
			var C = 1e-9;
			if (Math.abs(h - this.prevCamera.lat) > C || Math.abs(p - this.prevCamera.lng) > C || Math.abs(d - this.prevCamera.alt) > C || Math.abs(this.cameraHeading - this.prevCameraHeading) > C || Math.abs(m - this.prevCameraTilt) > C || Math.abs(g - this.prevCameraRoll) > C) this.prevCamera.lat = h,
			this.prevCamera.lng = p,
			this.prevCamera.alt = d,
			this.prevCameraHeading = this.cameraHeading,
			this.prevCameraTilt = m,
			this.prevCameraRoll = g,
			this.kmlCamera.set(h, p, d, this.ge.ALTITUDE_RELATIVE_TO_GROUND, this.cameraHeading * 57.29577951308232, m * 57.29577951308232, g * 57.29577951308232),
			this.ge.getView().setAbstractView(this.kmlCamera)
		},
		updateVehicleModels: function (e) {
			var t = this,
			n = this.getAltitude(this.vehiclePosition.createOffset(this.getVehicleHeading(), 2)),
			r = this.getAltitude(this.vehiclePosition.createOffset(this.getVehicleHeading(), -2));
			this.terrainTilt = -Math.atan((n - r) / 4);
			var i = this.getAltitude(this.vehiclePosition.createOffset(this.getVehicleHeading() - 1.570796326794897, .75)),
			s = this.getAltitude(this.vehiclePosition.createOffset(this.getVehicleHeading() + 1.570796326794897, .75));
			this.terrainRoll = Math.atan((s - i) / 1.5),
			this.isSpawning ? (this.vehicleTilt.position = this.terrainTilt, this.vehicleRoll.position = this.terrainRoll) : (this.vehicleTilt.targetPosition = this.terrainTilt, this.vehicleTilt.update(e), this.vehicleRoll.targetPosition = this.terrainRoll, this.vehicleRoll.update(e));
			var o = this.vehiclePosition.lat,
			u = this.vehiclePosition.lng,
			a = this.getAltitude(this.vehiclePosition),
			f = v.geoquake.Angle.wrap(this.getVehicleHeading()),
			l = this.vehicleTilt.position,
			c = this.vehicleRoll.position;
			console.log('vehicle Tile('+l+') Roll('+c+')');
			this.vehicle.setAttitude( - l, c),
			this.bodyModel.setLocation(o, u, a, f, l, c),
			this.windowModel.setLocation(o, u, a, f, l, c);
			var h = this.vehicle.getSpeedKmh() * (1e3 / 3600) * e / this.vehicleSpec.wheelRadius;
			this.vehicle.getGear() == -1 && (h = -h),
			h = v.geoquake.MyMath.clamp(h, -0.34906585039886595, .34906585039886595),
			this.wheelAngle += h;
			var p = v.geoquake.geometry.Quaternion.euler(f, l, c),
			d = function (e, n, r, i, s, o) {
				var u = t.getLatLngAlt(n, r, i),
				a = o,
				f = t.wheelAngle;
				s && (a += 3.141592653589794, f = -f);
				var l = v.geoquake.geometry.Quaternion.angleAxis(a, new v.geoquake.geometry.Vector3(0, 0, 1)),
				c = v.geoquake.geometry.Quaternion.angleAxis(f, new v.geoquake.geometry.Vector3(1, 0, 0)),
				h = c.mul(l).mul(p).get_eulerAngles();
				t.wheelModels[e].setLocation(u.lat, u.lng, u.alt, h.heading, h.pitch, h.bank)
			},
			m = this.vehicleSpec.tread / 2,
			g = -this.vehicleSpec.rearY,
			y = g + this.vehicleSpec.wheelBase,
			b = this.vehicleSpec.wheelRadius;
			d(0, -m, y, b, !0, this.vehicle.frontLAngle),
			d(1, m, y, b, !1, this.vehicle.frontRAngle),
			d(2, -m, g, b, !0, 0),
			d(3, m, g, b, !1, 0);
			var w = v.geoquake.geometry.Quaternion.angleAxis( - 1.3962634015954638, new v.geoquake.geometry.Vector3(1, 0, 0)),
			E = v.geoquake.geometry.Quaternion.angleAxis(this.vehicle.getSteeringAngle(), new v.geoquake.geometry.Vector3(0, 0, 1)),
			S = E.mul(w).mul(p).get_eulerAngles(),
			x = this.getLatLngAlt(.35 * (this.isRightHandDrive ? 1 : -1), .75, 1.2 + -0.32);
			this.steeringModel.setLocation(x.lat, x.lng, x.alt, S.heading, S.pitch, S.bank),
			this.shadow.setLatLngRot(this.vehiclePosition, -this.getVehicleHeading(), 2.275)
		},
		getAltitude: function (e) {
			return this.ge.getGlobe().getGroundAltitude(e.lat, e.lng)
		},
		batchedUpdate: function () {
			this.prevTime = this.currentTime,
			this.currentTime = (new Date).getTime();
			var e = v.geoquake.MyMath.clamp(this.currentTime - this.prevTime, 1, 50) / 1e3;
			this.isSpawning = (new Date).getTime() < this.startTime + 5e3,
			this.virtualInput.update(e),
			this.virtualInput.changeViewPointPressed() && this.changeViewPoint(),
			this.virtualInput.switchDriverSheetPressed() && (this.isRightHandDrive = !this.isRightHandDrive, this.changeViewPoint(0)),
			this.virtualInput.turnSignalLeftPressed() && this.vehicle.switchLeftTurnSignal(),
			this.virtualInput.turnSignalRightPressed() && this.vehicle.switchRightTurnSignal(),
			this.virtualInput.turnSignalCancelPressed() && this.vehicle.cancelTurnSignal(),
			this.virtualInput.hazardPressed() && this.vehicle.toggleHazard(),
			this.virtualInput.gearUpPressed() && this.vehicle.gearUp(),
			this.virtualInput.gearDownPressed() && this.vehicle.gearDown();
			if (this.virtualInput.displayPositionPressed()) {
				var t = 6;
				v.geoquake.js.Debug.log(v.geoquake.MyMath.round(this.vehiclePosition.lat, t) + ", " + v.geoquake.MyMath.round(this.vehiclePosition.lng, t) + ", " + v.geoquake.MyMath.round(this.getVehicleHeading(), t))
			}
			this.prevPosition.copy(this.currentPosition),
			this.updatePhysics(e),
			this.currentPosition = this.vehicle.getPosition();
			var n = this.currentPosition.x - this.prevPosition.x,
			i = this.currentPosition.y - this.prevPosition.y,
			s = Math.sqrt(n * n + i * i),
			o = Math.atan2(i, n) + 1.570796326794897;
			this.vehiclePosition = this.vehiclePosition.createOffset(o, s),
			this.updateVehicleModels(e),
			this.updateCamera(e),
			this.updateLights(e),
			this.updateSpeedMeter(),
			this.steering.setRotation( - this.vehicle.getSteeringAngle() * 57.29577951308232);
			var u = Math.round(this.vehicle.getSpeedKmh()),
			a = 1;
			u < 20 || this.vehicle.getGear() == -1 ? a = v.geoquake.MyMath.linearMap(u, 0, 30, 1, 1.8) : u < 40 ? a = v.geoquake.MyMath.linearMap(u, 20, 40, 1.2, 1.8) : u < 60 ? a = v.geoquake.MyMath.linearMap(u, 40, 60, 1.4, 1.8) : a = v.geoquake.MyMath.linearMap(u, 60, 100, 1.5, 1.8),
			a -= .5,
			r.setDeltaPhase(0, a),
			r.setVolume(0, 1);
			var f = this.vehicle.getSquealLevel();
			this.isSpawning && (f = 0),
			r.setVolume(1, f)
		},
		update: function () {
			v.geoquake.js.Debug.update();
			try {
				google.earth.executeBatch(this.ge, E(this, this.batchedUpdate))
			} catch(e) {
				v.geoquake.js.Debug.log(e);
				return
			}
			this.enableDebugCanvas && this.world.drawDebugData()
		},
		updatePhysics: function (e) {
			this.vehicle.setSteeringLever(this.virtualInput.getSteeringDelta()),
			this.vehicle.setThrottle(this.virtualInput.getThrottle()),
			this.vehicle.setBrake(this.virtualInput.getBrake()),
			this.vehicle.update(e),
			this.world.step(e, 8, 3),
			this.world.clearForces(),
			this.vehicle.lateUpdate(e)
		},
		setupPhysics: function (e) {
			this.world = new d.dynamics.B2World(new d.common.math.B2Vec2(0, 0), !0),
			this.vehicleSpec = new b.VehicleSpec,
			this.vehicleSpec.bodyWidth = 1.81,
			this.vehicleSpec.bodyLength = 4.495,
			this.vehicleSpec.rearY = 1.296,
			this.vehicleSpec.wheelBase = 2.65,
			this.vehicleSpec.tread = 1.4,
			this.vehicleSpec.wheelRadius = .3106;
			var t = a.getCarVertices();
			this.vehicle = new b.Vehicle(this.world, t, this.vehicleSpec, 0, 0, e, !0),
			this.prevPosition = this.vehicle.getPosition(),
			this.currentPosition = this.vehicle.getPosition();
			var n = y.Browser.document.getElementById("debug_canvas");
			if (n != null) {
				this.enableDebugCanvas = !0;
				var r = n.getContext("2d"),
				i = new d.dynamics.B2DebugDraw;
				i.setDrawScale(10),
				i.setCanvas(n),
				i.setFillAlpha(.3),
				i.setLineThickness(1),
				i.setFlags(d.dynamics.B2DebugDraw.e_shapeBit | d.dynamics.B2DebugDraw.e_jointBit),
				this.world.setDebugDraw(i)
			}
			var s = 0;
			while (s < 100) {
				var o = s++;
				this.updatePhysics(1 / 60)
			}
		},
		setupVehicle: function (e, t, n) {
			this.vehiclePosition = new s(e, t),
			this.vehicleTilt = new b.Suspension(0, 0),
			this.vehicleRoll = new b.Suspension(0, 0),
			this.setupPhysics(n),
			this.cameraHeadingBase = this.getVehicleHeading(),
			this.startTime = (new Date).getTime(),
			this.setKeyFocus()
		},
		setupKeyboard: function () {
			google.earth.addEventListener(this.ge.getWindow(), "mouseup", E(this, this.onMouseUp));
			var e = y.Browser.document.body;
			v.geoquake.js.Platform.isFirefox() && (e = y.Browser.document.getElementById("Firefox_keyfocus_hack")),
			this.virtualInput.setup(e)
		},
		setKeyFocus: function () {
			v.geoquake.js.Platform.isIE() ? y.Browser.document.body.focus() : v.geoquake.js.Platform.isChrome() ? (y.Browser.document.activeElement.blur(), this.ge.getWindow().blur()) : v.geoquake.js.Platform.isFirefox() && y.Browser.document.getElementById("Firefox_keyfocus_hack").focus(),
			this.setActive(!0)
		},
		onMouseUp: function () {
			this.setKeyFocus()
		},
		changeViewPoint: function (e) {
			e != null ? this.viewPointIndex = e: ++this.viewPointIndex >= this.viewPoints.length && (this.viewPointIndex = 0),
			this.viewPointIndex == 0 ? (this.windowModel.setVisibility(!1), this.steering.setVisibility(!1)) : (this.windowModel.setVisibility(!0), this.steering.setVisibility(!0))
		},
		onLoad: function () {
			this.windowModel = new m.Model(this.ge, u.modelsURL + "window.kmz"),
			this.windowModel.load(),
			this.bodyModel = new m.Model(this.ge, u.modelsURL + "body.kmz"),
			this.bodyModel.load(),
			this.wheelModels = [];
			var e = 0;
			while (e < 4) {
				var t = e++;
				this.wheelModels[t] = new m.Model(this.ge, u.modelsURL + "wheel.kmz"),
				this.wheelModels[t].load()
			}
			this.steeringModel = new m.Model(this.ge, u.modelsURL + "steering.kmz"),
			this.steeringModel.load(),
			this.shadow = new m.GroundOverlay(this.ge, u.imagesURL + "shadow.png"),
			this.steering = new m.Sprite(this.ge, u.imagesURL + "steering.png"),
			this.steering.setPosition(20, 20, 0, 0),
			this.steering.setDrawOrder(100),
			this.brakeLightL = new m.Sprite(this.ge, u.imagesURL + "light_brake.png"),
			this.brakeLightR = new m.Sprite(this.ge, u.imagesURL + "light_brake.png"),
			this.reverseLightL = new m.Sprite(this.ge, u.imagesURL + "light_reverse.png"),
			this.reverseLightR = new m.Sprite(this.ge, u.imagesURL + "light_reverse.png"),
			this.turnSignalLightFL = new m.Sprite(this.ge, u.imagesURL + "light_turnsignal.png"),
			this.turnSignalLightFR = new m.Sprite(this.ge, u.imagesURL + "light_turnsignal.png"),
			this.turnSignalLightRL = new m.Sprite(this.ge, u.imagesURL + "light_turnsignal.png"),
			this.turnSignalLightRR = new m.Sprite(this.ge, u.imagesURL + "light_turnsignal.png"),
			this.driverTurnSignalL = new m.Sprite(this.ge, u.imagesURL + "DriversEyeTurnSignalLeft.png"),
			this.driverTurnSignalR = new m.Sprite(this.ge, u.imagesURL + "DriversEyeTurnSignalRight.png"),
			this.driverTurnSignalL.setPosition(204, 50, .5, 0),
			this.driverTurnSignalR.setPosition(564, 50, .5, 0),
			this.brakeLightL.setOpacity(0),
			this.brakeLightR.setOpacity(0),
			this.reverseLightL.setOpacity(0),
			this.reverseLightR.setOpacity(0),
			this.turnSignalLightFL.setOpacity(0),
			this.turnSignalLightFR.setOpacity(0),
			this.turnSignalLightRL.setOpacity(0),
			this.turnSignalLightRR.setOpacity(0),
			this.driverTurnSignalL.setOpacity(0),
			this.driverTurnSignalR.setOpacity(0);
			var n = 686,
			r = 120;
			this.speedKmhSpriteNumber = new m.SpriteNumber(this.ge, 569, r, 39, 34, 3, u.imagesURL + "digit_kmh"),
			this.speedMphSpriteNumber = new m.SpriteNumber(this.ge, 603, r - 36, 27, 24, 3, u.imagesURL + "digit_mph");
			var i = new m.Sprite(this.ge, u.imagesURL + "units.png");
			i.setPosition(686, r - 39, 0, 0),
			this.gravityCircle = new m.Sprite(this.ge, u.imagesURL + "gravity_circle.png"),
			this.gravityMarker = new m.Sprite(this.ge, u.imagesURL + "gravity_marker.png"),
			this.gravityCircle.setPosition(616, 210, 0, 0),
			this.gravityMarker.setPosition(664, 258, 0, 0),
			this.setupVehicle(21.309101, -157.849663, 17.842295),
			this.prevCamera = new o(0, 0, 0),
			v.geoquake.js.Debug.log("onLoad finished"),
			this.currentTime = (new Date).getTime(),
			v.geoquake.js.Debug.log("Setup Frameend Handler"),
			google.earth.addEventListener(this.ge, "frameend", E(this, this.update))
		},
		onSuccess: function (e) {
			v.geoquake.js.Debug.log("onSuccess"),
			v.geoquake.js.Debug.log(e),
			this.ge = e,
			e.getLayerRoot().enableLayerById(e.LAYER_BUILDINGS, !0),
			e.getOptions().setMouseNavigationEnabled(!1),
			e.getOptions().setFlyToSpeed(e.SPEED_TELEPORT),
			e.getOptions().setFadeInOutEnabled(!1),
			e.getWindow().setVisibility(!0),
			this.kmlCamera = e.getView().copyAsCamera(e.ALTITUDE_RELATIVE_TO_GROUND),
			this.setupKeyboard(),
			this.onLoad()
		},
		init: function () {
			v.geoquake.js.Debug.log("init"),
			google.earth.createInstance("map3d", E(this, this.onSuccess), function () {})
		},
		__class__: n
	};
	var r = function () {};
	r.__name__ = !0,
	r.getSWF = function () {
		return y.Browser.document.getElementById("Flash")
	},
	r.playSE = function (e) {
		var t = r.getSWF();
		t && f.isFunction(t.playSE) && t.playSE(e)
	},
	r.setDeltaPhase = function (e, t) {
		var n = r.getSWF();
		n && f.isFunction(n.setDeltaPhase) && n.setDeltaPhase(e, t)
	},
	r.setVolume = function (e, t) {
		var n = r.getSWF();
		n && f.isFunction(n.setVolume) && n.setVolume(e, t)
	};
	var i = function () {};
	i.__name__ = !0,
	i.dateStr = function (e) {
		var t = e.getMonth() + 1,
		n = e.getDate(),
		r = e.getHours(),
		i = e.getMinutes(),
		s = e.getSeconds();
		return e.getFullYear() + "-" + (t < 10 ? "0" + t: "" + t) + "-" + (n < 10 ? "0" + n: "" + n) + " " + (r < 10 ? "0" + r: "" + r) + ":" + (i < 10 ? "0" + i: "" + i) + ":" + (s < 10 ? "0" + s: "" + s)
	},
	i.cca = function (e, t) {
		var n = e.charCodeAt(t);
		return n != n ? undefined: n
	},
	i.substr = function (e, t, n) {
		return t != null && t != 0 && n != null && n < 0 ? "": (n == null && (n = e.length), t < 0 ? (t = e.length + t, t < 0 && (t = 0)) : n < 0 && (n = e.length + n - t), e.substr(t, n))
	},
	i.remove = function (e, t) {
		var n = 0,
		r = e.length;
		while (n < r) {
			if (e[n] == t) return e.splice(n, 1),
			!0;
			n++
		}
		return ! 1
	},
	i.iter = function (e) {
		return {
			cur: 0,
			arr: e,
			hasNext: function () {
				return this.cur < this.arr.length
			},
			next: function () {
				return this.arr[this.cur++]
			}
		}
	};
	var s = function (e, t) {
		this.lat = e,
		this.lng = t
	};
	s.__name__ = !0,
	s.prototype = {
		distance: function (e) {
			var t = Math.sin(this.lat * .0174532925199433) * Math.sin(e.lat * .0174532925199433),
			n = Math.cos(this.lat * .0174532925199433) * Math.cos(e.lat * .0174532925199433) * Math.cos((e.lng - this.lng) * .0174532925199433);
			return 6371e3 * Math.acos(t + n)
		},
		createOffset: function (e, t) {
			var n = this.lat * .0174532925199433,
			r = this.lng * .0174532925199433;
			t /= 6371e3;
			var i = Math.asin(Math.sin(n) * Math.cos(t) + Math.cos(n) * Math.sin(t) * Math.cos(e)),
			o = r + Math.atan2(Math.sin(e) * Math.sin(t) * Math.cos(i), Math.cos(t) - Math.sin(n) * Math.sin(i));
			return new s(i * 57.29577951308232, o * 57.29577951308232)
		},
		copy: function () {
			return new s(this.lat, this.lng)
		},
		__class__: s
	};
	var o = function (e, t, n) {
		this.lat = e,
		this.lng = t,
		this.alt = n
	};
	o.__name__ = !0,
	o.prototype = {
		copy: function (e) {
			this.lat = e.lat,
			this.lng = e.lng,
			this.alt = e.alt
		},
		set: function (e, t, n) {
			this.lat = e,
			this.lng = t,
			this.alt = n
		},
		__class__: o
	};
	var u = function () {};
	u.__name__ = !0,
	u.main = function () {
		v.geoquake.js.Debug.log("3D Driving Simulator on Google Earth version 1.04 (" + i.dateStr(new Date) + ")"),
		u.baseURL = y.Browser.window.location.href;
		var e = u.baseURL.lastIndexOf("index.html");
		e >= 0 && (u.baseURL = i.substr(u.baseURL, 0, e)),
		//m.Model.expirationCode = "1.04",
		m.Model.expirationCode = null;
		u.modelsURL = u.baseURL + "models/",
		u.imagesURL = u.baseURL + "images/";
		var t = new v.geoquake.js.DomainChecker;
		t.registerDomain("geoquake.jp"),
		t.registerDomain("www.geoquake.com"),
		t.allowPrivateNetwork();
		if (!t.check()) return;
		v.geoquake.js.Debug.log("Initialize driving simulator"),
		u.drivingSimulator = new n,
		v.geoquake.js.Debug.log("Load Google Earth"),
		google.load("earth", "1", {
			other_params: "sensor=false",
			callback: function () {
				v.geoquake.js.Debug.log("Load Google Earth Succeeded"),
				u.drivingSimulator.init()
			}
		}),
		u.geocoder = new google.maps.Geocoder
	},
	u["goto"] = function (e, t, n) {
		u.drivingSimulator.setupVehicle(e, t, n)
	},
	L(u["goto"], "Main.goto"),
	u.geocode = function (e) {
		u.geocoder.geocode({
			address: e
		},
		function (e, t) {
			if (t == google.maps.GeocoderStatus.OK) {
				var n = e[0].geometry.location,
				r = n.lat(),
				i = n.lng();
				u["goto"](r, i, 0)
			}
		})
	},
	L(u.geocode, "Main.geocode"),
	u.activate = function () {
		u.drivingSimulator.setActive(!0)
	},
	L(u.activate, "Main.activate"),
	u.deactivate = function () {
		u.drivingSimulator.setActive(!1)
	},
	L(u.deactivate, "Main.deactivate");
	var a = function () {};
	a.__name__ = !0,
	a.createCarVertices = function () {
		var e = [[.397, -2.149], [.835, -1.662], [.835, 1.295], [.81, 1.88], [.704, 2.151]],
		t = [],
		n = 0;
		while (n < e.length) {
			var r = e[n]; ++n,
			t.push(new d.common.math.B2Vec2(r[0], r[1]))
		}
		var i = e.length;
		while (--i >= 0) {
			var r = e[i];
			t.push(new d.common.math.B2Vec2( - r[0], r[1]))
		}
		a.carVertices = t
	},
	a.getCarVertices = function () {
		return a.carVertices == null && a.createCarVertices(),
		a.carVertices
	};
	var f = function () {};
	f.__name__ = !0,
	f.isFunction = function (e) {
		return typeof e == "function" && !e.__name__ && !e.__ename__
	};
	var l = function () {};
	l.__name__ = !0,
	l.string = function (e) {
		return y.Boot.__string_rec(e, "")
	},
	l.random = function (e) {
		return e <= 0 ? 0 : Math.floor(Math.random() * e)
	};
	var c = function () {};
	c.__name__ = !0,
	c.hex = function (e, t) {
		var n = "",
		r = "0123456789ABCDEF";
		do n = r.charAt(e & 15) + n,
		e >>>= 4;
		while (e > 0);
		if (t != null) while (n.length < t) n = "0" + n;
		return n
	};
	var h = function (e, t, n) {
		this.altitude = e,
		this.offset = t,
		this.tilt = n
	};
	h.__name__ = !0,
	h.prototype = {
		__class__: h
	};
	var p = function () {
		this.gearDownCount = 0,
		this.gearUpCount = 0,
		this.viewY = 0,
		this.viewX = 0,
		this.displayPositionCount = 0,
		this.switchDriverSheetCount = 0,
		this.hazardCount = 0,
		this.turnSignalCancelCount = 0,
		this.turnSignalRightCount = 0,
		this.turnSignalLeftCount = 0,
		this.changeViewPointCount = 0,
		this.brake = 0,
		this.throttle = 0,
		this.steeringDelta = 0
	};
	p.__name__ = !0,
	p.prototype = {
		setActive: function (e) {
			this.keyboard.active = e
		},
		getViewY: function () {
			return this.viewY
		},
		getViewX: function () {
			return this.viewX
		},
		displayPositionPressed: function () {
			return this.displayPositionCount == 1
		},
		switchDriverSheetPressed: function () {
			return this.switchDriverSheetCount == 1
		},
		hazardPressed: function () {
			return this.hazardCount == 1
		},
		turnSignalCancelPressed: function () {
			return this.turnSignalCancelCount == 1
		},
		turnSignalRightPressed: function () {
			return this.turnSignalRightCount == 1
		},
		turnSignalLeftPressed: function () {
			return this.turnSignalLeftCount == 1
		},
		changeViewPointPressed: function () {
			return this.changeViewPointCount == 1
		},
		gearDownPressed: function () {
			return this.gearDownCount == 1
		},
		gearUpPressed: function () {
			return this.gearUpCount == 1
		},
		getBrake: function () {
			return this.brake
		},
		getThrottle: function () {
			return this.throttle
		},
		getSteeringDelta: function () {
			return this.steeringDelta
		},
		update: function (e) {
			this.steeringDelta = v.geoquake.js.GamePad.getAxis(0),
			Math.abs(this.steeringDelta) < .3 && (this.steeringDelta = 0),
			this.keyboard.getStickX() != 0 && (this.steeringDelta = this.keyboard.getStickX()),
			this.throttle = Math.max(v.geoquake.js.GamePad.getButton(7), v.geoquake.js.GamePad.getButton(0)),
			this.throttle < .3 && (this.throttle = 0),
			this.keyboard.getStickY() > 0 && (this.throttle = 1),
			this.brake = Math.max(v.geoquake.js.GamePad.getButton(6), v.geoquake.js.GamePad.getButton(1)),
			this.brake < .3 && (this.brake = 0),
			this.keyboard.getStickY() < 0 && (this.brake = 1),
			this.keyboard.isDown(i.cca("V", 0)) || v.geoquake.js.GamePad.getButton(8) > 0 ? this.changeViewPointCount++:this.changeViewPointCount = 0,
			this.keyboard.isDown(46) || this.keyboard.isDown(188) || v.geoquake.js.GamePad.getButton(14) > 0 ? this.turnSignalLeftCount++:this.turnSignalLeftCount = 0,
			this.keyboard.isDown(34) || this.keyboard.isDown(190) || v.geoquake.js.GamePad.getButton(15) > 0 ? this.turnSignalRightCount++:this.turnSignalRightCount = 0,
			this.keyboard.isDown(35) ? this.turnSignalCancelCount++:this.turnSignalCancelCount = 0,
			this.keyboard.isDown(i.cca("H", 0)) || v.geoquake.js.GamePad.getButton(13) > 0 ? this.hazardCount++:this.hazardCount = 0,
			this.keyboard.isDown(i.cca("X", 0)) ? this.switchDriverSheetCount++:this.switchDriverSheetCount = 0,
			this.keyboard.isDown(i.cca("P", 0)) ? this.displayPositionCount++:this.displayPositionCount = 0,
			v.geoquake.js.GamePad.getButton(5) > 0 ? this.gearUpCount++:this.gearUpCount = 0,
			v.geoquake.js.GamePad.getButton(4) > 0 ? this.gearDownCount++:this.gearDownCount = 0,
			this.viewX = 0,
			this.viewY = 0;
			var t = v.geoquake.js.GamePad.getAxis(2),
			n = v.geoquake.js.GamePad.getAxis(3);
			t * t + n * n > .09 && (this.viewX = t, this.viewY = -n),
			this.keyboard.isDown(i.cca("W", 0)) && (this.viewY = 1),
			this.keyboard.isDown(i.cca("D", 0)) && (this.viewX = 1),
			this.keyboard.isDown(i.cca("A", 0)) && (this.viewX = -1),
			this.keyboard.isDown(i.cca("S", 0)) && (this.viewY = -1)
		},
		setup: function (e) {
			this.keyboard = new v.geoquake.js.Keyboard(e)
		},
		__class__: p
	};
	var d = {};
	d.collision = {},
	d.collision.B2AABB = function () {
		this.lowerBound = new d.common.math.B2Vec2,
		this.upperBound = new d.common.math.B2Vec2
	},
	d.collision.B2AABB.__name__ = !0,
	d.collision.B2AABB.prototype = {
		combine: function (e, t) {
			this.lowerBound.x = Math.min(e.lowerBound.x, t.lowerBound.x),
			this.lowerBound.y = Math.min(e.lowerBound.y, t.lowerBound.y),
			this.upperBound.x = Math.max(e.upperBound.x, t.upperBound.x),
			this.upperBound.y = Math.max(e.upperBound.y, t.upperBound.y)
		},
		testOverlap: function (e) {
			var t = e.lowerBound.x - this.upperBound.x,
			n = e.lowerBound.y - this.upperBound.y,
			r = this.lowerBound.x - e.upperBound.x,
			i = this.lowerBound.y - e.upperBound.y;
			return t > 0 || n > 0 ? !1 : r > 0 || i > 0 ? !1 : !0
		},
		rayCast: function (e, t) {
			var n = -1.7976931348623157e308,
			r = 1.7976931348623157e308,
			i = t.p1.x,
			s = t.p1.y,
			o = t.p2.x - t.p1.x,
			u = t.p2.y - t.p1.y,
			a = Math.abs(o),
			f = Math.abs(u),
			l = e.normal,
			c,
			h,
			p,
			d,
			v;
			if (a < 2.2250738585072014e-308) {
				if (i < this.lowerBound.x || this.upperBound.x < i) return ! 1
			} else {
				c = 1 / o,
				h = (this.lowerBound.x - i) * c,
				p = (this.upperBound.x - i) * c,
				v = -1,
				h > p && (d = h, h = p, p = d, v = 1),
				h > n && (l.x = v, l.y = 0, n = h),
				r = Math.min(r, p);
				if (n > r) return ! 1
			}
			if (f < 2.2250738585072014e-308) {
				if (s < this.lowerBound.y || this.upperBound.y < s) return ! 1
			} else {
				c = 1 / u,
				h = (this.lowerBound.y - s) * c,
				p = (this.upperBound.y - s) * c,
				v = -1,
				h > p && (d = h, h = p, p = d, v = 1),
				h > n && (l.y = v, l.x = 0, n = h),
				r = Math.min(r, p);
				if (n > r) return ! 1
			}
			return e.fraction = n,
			!0
		},
		contains: function (e) {
			var t = !0;
			return t = t && this.lowerBound.x <= e.lowerBound.x,
			t = t && this.lowerBound.y <= e.lowerBound.y,
			t = t && e.upperBound.x <= this.upperBound.x,
			t = t && e.upperBound.y <= this.upperBound.y,
			t
		},
		getExtents: function () {
			return new d.common.math.B2Vec2((this.upperBound.x - this.lowerBound.x) / 2, (this.upperBound.y - this.lowerBound.y) / 2)
		},
		getCenter: function () {
			return new d.common.math.B2Vec2((this.lowerBound.x + this.upperBound.x) / 2, (this.lowerBound.y + this.upperBound.y) / 2)
		},
		isValid: function () {
			var e = this.upperBound.x - this.lowerBound.x,
			t = this.upperBound.y - this.lowerBound.y,
			n = e >= 0 && t >= 0;
			return n = n && this.lowerBound.isValid() && this.upperBound.isValid(),
			n
		},
		__class__: d.collision.B2AABB
	},
	d.common = {},
	d.common.math = {},
	d.common.math.B2Vec2 = function (e, t) {
		t == null && (t = 0),
		e == null && (e = 0),
		this.x = e,
		this.y = t
	},
	d.common.math.B2Vec2.__name__ = !0,
	d.common.math.B2Vec2.make = function (e, t) {
		return new d.common.math.B2Vec2(e, t)
	},
	d.common.math.B2Vec2.prototype = {
		isValid: function () {
			return d.common.math.B2Math.isValid(this.x) && d.common.math.B2Math.isValid(this.y)
		},
		normalize: function () {
			var e = Math.sqrt(this.x * this.x + this.y * this.y);
			if (e < 2.2250738585072014e-308) return 0;
			var t = 1 / e;
			return this.x *= t,
			this.y *= t,
			e
		},
		lengthSquared: function () {
			return this.x * this.x + this.y * this.y
		},
		length: function () {
			return Math.sqrt(this.x * this.x + this.y * this.y)
		},
		abs: function () {
			this.x < 0 && (this.x = -this.x),
			this.y < 0 && (this.y = -this.y)
		},
		maxV: function (e) {
			this.x = this.x > e.x ? this.x: e.x,
			this.y = this.y > e.y ? this.y: e.y
		},
		minV: function (e) {
			this.x = this.x < e.x ? this.x: e.x,
			this.y = this.y < e.y ? this.y: e.y
		},
		crossFV: function (e) {
			var t = this.x;
			this.x = -e * this.y,
			this.y = e * t
		},
		crossVF: function (e) {
			var t = this.x;
			this.x = e * this.y,
			this.y = -e * t
		},
		mulTM: function (e) {
			var t = d.common.math.B2Math.dot(this, e.col1);
			this.y = d.common.math.B2Math.dot(this, e.col2),
			this.x = t
		},
		mulM: function (e) {
			var t = this.x;
			this.x = e.col1.x * t + e.col2.x * this.y,
			this.y = e.col1.y * t + e.col2.y * this.y
		},
		multiply: function (e) {
			this.x *= e,
			this.y *= e
		},
		subtract: function (e) {
			this.x -= e.x,
			this.y -= e.y
		},
		add: function (e) {
			this.x += e.x,
			this.y += e.y
		},
		copy: function () {
			return new d.common.math.B2Vec2(this.x, this.y)
		},
		negativeSelf: function () {
			this.x = -this.x,
			this.y = -this.y
		},
		getNegative: function () {
			return new d.common.math.B2Vec2( - this.x, -this.y)
		},
		setV: function (e) {
			this.x = e.x,
			this.y = e.y
		},
		set: function (e, t) {
			t == null && (t = 0),
			e == null && (e = 0),
			this.x = e,
			this.y = t
		},
		setZero: function () {
			this.x = 0,
			this.y = 0
		},
		__class__: d.common.math.B2Vec2
	},
	d.collision.ClipVertex = function () {
		this.v = new d.common.math.B2Vec2,
		this.id = new d.collision.B2ContactID
	},
	d.collision.ClipVertex.__name__ = !0,
	d.collision.ClipVertex.prototype = {
		set: function (e) {
			this.v.setV(e.v),
			this.id.set(e.id)
		},
		__class__: d.collision.ClipVertex
	},
	d.collision.B2ContactID = function () {
		this.features = new d.collision.Features,
		this.features._m_id = this
	},
	d.collision.B2ContactID.__name__ = !0,
	d.collision.B2ContactID.prototype = {
		set_key: function (e) {
			return this._key = e,
			this.features._referenceEdge = this._key & 255,
			this.features._incidentEdge = (this._key & 65280) >> 8 & 255,
			this.features._incidentVertex = (this._key & 16711680) >> 16 & 255,
			this.features._flip = (this._key & -16777216) >> 24 & 255,
			this._key
		},
		get_key: function () {
			return this._key
		},
		copy: function () {
			var e = new d.collision.B2ContactID;
			return e.set_key(this.get_key()),
			e
		},
		set: function (e) {
			this.set_key(e._key)
		},
		__class__: d.collision.B2ContactID
	},
	d.collision.Features = function () {},
	d.collision.Features.__name__ = !0,
	d.collision.Features.prototype = {
		set_flip: function (e) {
			return this._flip = e,
			this._m_id._key = this._m_id._key & 16777215 | this._flip << 24 & -16777216,
			e
		},
		get_flip: function () {
			return this._flip
		},
		set_incidentVertex: function (e) {
			return this._incidentVertex = e,
			this._m_id._key = this._m_id._key & -16711681 | this._incidentVertex << 16 & 16711680,
			e
		},
		get_incidentVertex: function () {
			return this._incidentVertex
		},
		set_incidentEdge: function (e) {
			return this._incidentEdge = e,
			this._m_id._key = this._m_id._key & -65281 | this._incidentEdge << 8 & 65280,
			e
		},
		get_incidentEdge: function () {
			return this._incidentEdge
		},
		set_referenceEdge: function (e) {
			return this._referenceEdge = e,
			this._m_id._key = this._m_id._key & -256 | this._referenceEdge & 255,
			e
		},
		get_referenceEdge: function () {
			return this._referenceEdge
		},
		__class__: d.collision.Features
	},
	d.collision.B2Collision = function () {},
	d.collision.B2Collision.__name__ = !0,
	d.collision.B2Collision.clipSegmentToLine = function (e, t, n, r) {
		var i, s = 0;
		i = t[0];
		var o = i.v;
		i = t[1];
		var u = i.v,
		a = n.x * o.x + n.y * o.y - r,
		f = n.x * u.x + n.y * u.y - r;
		a <= 0 && e[s++].set(t[0]),
		f <= 0 && e[s++].set(t[1]);
		if (a * f < 0) {
			var l = a / (a - f);
			i = e[s];
			var c = i.v;
			c.x = o.x + l * (u.x - o.x),
			c.y = o.y + l * (u.y - o.y),
			i = e[s];
			var h;
			a > 0 ? (h = t[0], i.id = h.id) : (h = t[1], i.id = h.id),
			++s
		}
		return s
	},
	d.collision.B2Collision.edgeSeparation = function (e, t, n, r, i) {
		var s = e.m_vertexCount,
		o = e.m_vertices,
		u = e.m_normals,
		a = r.m_vertexCount,
		f = r.m_vertices,
		l, c;
		l = t.R,
		c = u[n];
		var h = l.col1.x * c.x + l.col2.x * c.y,
		p = l.col1.y * c.x + l.col2.y * c.y;
		l = i.R;
		var d = l.col1.x * h + l.col1.y * p,
		v = l.col2.x * h + l.col2.y * p,
		m = 0,
		g = 1.7976931348623157e308,
		y = 0;
		while (y < a) {
			var b = y++;
			c = f[b];
			var w = c.x * d + c.y * v;
			w < g && (g = w, m = b)
		}
		c = o[n],
		l = t.R;
		var E = t.position.x + (l.col1.x * c.x + l.col2.x * c.y),
		S = t.position.y + (l.col1.y * c.x + l.col2.y * c.y);
		c = f[m],
		l = i.R;
		var x = i.position.x + (l.col1.x * c.x + l.col2.x * c.y),
		T = i.position.y + (l.col1.y * c.x + l.col2.y * c.y);
		x -= E,
		T -= S;
		var N = x * h + T * p;
		return N
	},
	d.collision.B2Collision.findMaxSeparation = function (e, t, n, r, i) {
		var s = t.m_vertexCount,
		o = t.m_normals,
		u, a;
		a = i.R,
		u = r.m_centroid;
		var f = i.position.x + (a.col1.x * u.x + a.col2.x * u.y),
		l = i.position.y + (a.col1.y * u.x + a.col2.y * u.y);
		a = n.R,
		u = t.m_centroid,
		f -= n.position.x + (a.col1.x * u.x + a.col2.x * u.y),
		l -= n.position.y + (a.col1.y * u.x + a.col2.y * u.y);
		var c = f * n.R.col1.x + l * n.R.col1.y,
		h = f * n.R.col2.x + l * n.R.col2.y,
		p = 0,
		v = -1.7976931348623157e308,
		m = 0;
		while (m < s) {
			var g = m++;
			u = o[g];
			var y = u.x * c + u.y * h;
			y > v && (v = y, p = g)
		}
		var b = d.collision.B2Collision.edgeSeparation(t, n, p, r, i),
		w = p - 1 >= 0 ? p - 1 : s - 1,
		E = d.collision.B2Collision.edgeSeparation(t, n, w, r, i),
		S = p + 1 < s ? p + 1 : 0,
		x = d.collision.B2Collision.edgeSeparation(t, n, S, r, i),
		T,
		N,
		C;
		if (E > b && E > x) C = -1,
		T = w,
		N = E;
		else {
			if (! (x > b)) return e[0] = p,
			b;
			C = 1,
			T = S,
			N = x
		}
		for (;;) {
			C == -1 ? p = T - 1 >= 0 ? T - 1 : s - 1 : p = T + 1 < s ? T + 1 : 0,
			b = d.collision.B2Collision.edgeSeparation(t, n, p, r, i);
			if (! (b > N)) break;
			T = p,
			N = b
		}
		return e[0] = T,
		N
	},
	d.collision.B2Collision.findIncidentEdge = function (e, t, n, r, i, s) {
		var o = t.m_vertexCount,
		u = t.m_normals,
		a = i.m_vertexCount,
		f = i.m_vertices,
		l = i.m_normals,
		c, h;
		c = n.R,
		h = u[r];
		var p = c.col1.x * h.x + c.col2.x * h.y,
		d = c.col1.y * h.x + c.col2.y * h.y;
		c = s.R;
		var v = c.col1.x * p + c.col1.y * d;
		d = c.col2.x * p + c.col2.y * d,
		p = v;
		var m = 0,
		g = 1.7976931348623157e308,
		y = 0;
		while (y < a) {
			var b = y++;
			h = l[b];
			var w = p * h.x + d * h.y;
			w < g && (g = w, m = b)
		}
		var E, S = m,
		x = S + 1 < a ? S + 1 : 0;
		E = e[0],
		h = f[S],
		c = s.R,
		E.v.x = s.position.x + (c.col1.x * h.x + c.col2.x * h.y),
		E.v.y = s.position.y + (c.col1.y * h.x + c.col2.y * h.y),
		E.id.features.set_referenceEdge(r),
		E.id.features.set_incidentEdge(S),
		E.id.features.set_incidentVertex(0),
		E = e[1],
		h = f[x],
		c = s.R,
		E.v.x = s.position.x + (c.col1.x * h.x + c.col2.x * h.y),
		E.v.y = s.position.y + (c.col1.y * h.x + c.col2.y * h.y),
		E.id.features.set_referenceEdge(r),
		E.id.features.set_incidentEdge(x),
		E.id.features.set_incidentVertex(1)
	},
	d.collision.B2Collision.makeClipPointVector = function () {
		var e = new Array;
		return e[0] = new d.collision.ClipVertex,
		e[1] = new d.collision.ClipVertex,
		e
	},
	d.collision.B2Collision.collidePolygons = function (e, t, n, r, i) {
		var s;
		e.m_pointCount = 0;
		var o = t.m_radius + r.m_radius,
		u = 0;
		d.collision.B2Collision.s_edgeAO[0] = u;
		var a = d.collision.B2Collision.findMaxSeparation(d.collision.B2Collision.s_edgeAO, t, n, r, i);
		u = d.collision.B2Collision.s_edgeAO[0];
		if (a > o) return;
		var f = 0;
		d.collision.B2Collision.s_edgeBO[0] = f;
		var l = d.collision.B2Collision.findMaxSeparation(d.collision.B2Collision.s_edgeBO, r, i, t, n);
		f = d.collision.B2Collision.s_edgeBO[0];
		if (l > o) return;
		var c, h, p, v, m, g, y = .98,
		b = .001,
		w;
		l > y * a + b ? (c = r, h = t, p = i, v = n, m = f, e.m_type = d.collision.B2Manifold.e_faceB, g = 1) : (c = t, h = r, p = n, v = i, m = u, e.m_type = d.collision.B2Manifold.e_faceA, g = 0);
		var E = d.collision.B2Collision.s_incidentEdge;
		d.collision.B2Collision.findIncidentEdge(E, c, p, m, h, v);
		var S = c.m_vertexCount,
		x = c.m_vertices,
		T = x[m],
		N;
		m + 1 < S ? N = x[m + 1 | 0] : N = x[0];
		var C = d.collision.B2Collision.s_localTangent;
		C.set(N.x - T.x, N.y - T.y),
		C.normalize();
		var k = d.collision.B2Collision.s_localNormal;
		k.x = C.y,
		k.y = -C.x;
		var L = d.collision.B2Collision.s_planePoint;
		L.set(.5 * (T.x + N.x), .5 * (T.y + N.y));
		var A = d.collision.B2Collision.s_tangent;
		w = p.R,
		A.x = w.col1.x * C.x + w.col2.x * C.y,
		A.y = w.col1.y * C.x + w.col2.y * C.y;
		var O = d.collision.B2Collision.s_tangent2;
		O.x = -A.x,
		O.y = -A.y;
		var M = d.collision.B2Collision.s_normal;
		M.x = A.y,
		M.y = -A.x;
		var _ = d.collision.B2Collision.s_v11,
		D = d.collision.B2Collision.s_v12;
		_.x = p.position.x + (w.col1.x * T.x + w.col2.x * T.y),
		_.y = p.position.y + (w.col1.y * T.x + w.col2.y * T.y),
		D.x = p.position.x + (w.col1.x * N.x + w.col2.x * N.y),
		D.y = p.position.y + (w.col1.y * N.x + w.col2.y * N.y);
		var P = M.x * _.x + M.y * _.y,
		H = -A.x * _.x - A.y * _.y + o,
		B = A.x * D.x + A.y * D.y + o,
		j = d.collision.B2Collision.s_clipPoints1,
		F = d.collision.B2Collision.s_clipPoints2,
		I;
		I = d.collision.B2Collision.clipSegmentToLine(j, E, O, H);
		if (I < 2) return;
		I = d.collision.B2Collision.clipSegmentToLine(F, j, A, B);
		if (I < 2) return;
		e.m_localPlaneNormal.setV(k),
		e.m_localPoint.setV(L);
		var q = 0,
		R = 0,
		U = d.common.B2Settings.b2_maxManifoldPoints;
		while (R < U) {
			var z = R++;
			s = F[z];
			var W = M.x * s.v.x + M.y * s.v.y - P;
			if (W <= o) {
				var X = e.m_points[q];
				w = v.R;
				var V = s.v.x - v.position.x,
				$ = s.v.y - v.position.y;
				X.m_localPoint.x = V * w.col1.x + $ * w.col1.y,
				X.m_localPoint.y = V * w.col2.x + $ * w.col2.y,
				X.m_id.set(s.id),
				X.m_id.features.set_flip(g),
				++q
			}
		}
		e.m_pointCount = q
	},
	d.collision.B2Collision.collideCircles = function (e, t, n, r, i) {
		e.m_pointCount = 0;
		var s, o;
		s = n.R,
		o = t.m_p;
		var u = n.position.x + (s.col1.x * o.x + s.col2.x * o.y),
		a = n.position.y + (s.col1.y * o.x + s.col2.y * o.y);
		s = i.R,
		o = r.m_p;
		var f = i.position.x + (s.col1.x * o.x + s.col2.x * o.y),
		l = i.position.y + (s.col1.y * o.x + s.col2.y * o.y),
		c = f - u,
		h = l - a,
		p = c * c + h * h,
		v = t.m_radius + r.m_radius;
		if (p > v * v) return;
		e.m_type = d.collision.B2Manifold.e_circles,
		e.m_localPoint.setV(t.m_p),
		e.m_localPlaneNormal.setZero(),
		e.m_pointCount = 1,
		e.m_points[0].m_localPoint.setV(r.m_p),
		e.m_points[0].m_id.set_key(0)
	},
	d.collision.B2Collision.collidePolygonAndCircle = function (e, t, n, r, i) {
		e.m_pointCount = 0;
		var s, o, u, a, f, l, c;
		c = i.R,
		l = r.m_p;
		var h = i.position.x + (c.col1.x * l.x + c.col2.x * l.y),
		p = i.position.y + (c.col1.y * l.x + c.col2.y * l.y);
		o = h - n.position.x,
		u = p - n.position.y,
		c = n.R;
		var v = o * c.col1.x + u * c.col1.y,
		m = o * c.col2.x + u * c.col2.y,
		g, y = 0,
		b = -1.7976931348623157e308,
		w = t.m_radius + r.m_radius,
		E = t.m_vertexCount,
		S = t.m_vertices,
		x = t.m_normals,
		T = 0;
		while (T < E) {
			var N = T++;
			l = S[N],
			o = v - l.x,
			u = m - l.y,
			l = x[N];
			var C = l.x * o + l.y * u;
			if (C > w) return;
			C > b && (b = C, y = N)
		}
		var k = y,
		L = k + 1 < E ? k + 1 : 0,
		A = S[k],
		O = S[L];
		if (b < 2.2250738585072014e-308) {
			e.m_pointCount = 1,
			e.m_type = d.collision.B2Manifold.e_faceA,
			e.m_localPlaneNormal.setV(x[y]),
			e.m_localPoint.x = .5 * (A.x + O.x),
			e.m_localPoint.y = .5 * (A.y + O.y),
			e.m_points[0].m_localPoint.setV(r.m_p),
			e.m_points[0].m_id.set_key(0);
			return
		}
		var M = (v - A.x) * (O.x - A.x) + (m - A.y) * (O.y - A.y),
		_ = (v - O.x) * (A.x - O.x) + (m - O.y) * (A.y - O.y);
		if (M <= 0) {
			if ((v - A.x) * (v - A.x) + (m - A.y) * (m - A.y) > w * w) return;
			e.m_pointCount = 1,
			e.m_type = d.collision.B2Manifold.e_faceA,
			e.m_localPlaneNormal.x = v - A.x,
			e.m_localPlaneNormal.y = m - A.y,
			e.m_localPlaneNormal.normalize(),
			e.m_localPoint.setV(A),
			e.m_points[0].m_localPoint.setV(r.m_p),
			e.m_points[0].m_id.set_key(0)
		} else if (_ <= 0) {
			if ((v - O.x) * (v - O.x) + (m - O.y) * (m - O.y) > w * w) return;
			e.m_pointCount = 1,
			e.m_type = d.collision.B2Manifold.e_faceA,
			e.m_localPlaneNormal.x = v - O.x,
			e.m_localPlaneNormal.y = m - O.y,
			e.m_localPlaneNormal.normalize(),
			e.m_localPoint.setV(O),
			e.m_points[0].m_localPoint.setV(r.m_p),
			e.m_points[0].m_id.set_key(0)
		} else {
			var D = .5 * (A.x + O.x),
			P = .5 * (A.y + O.y);
			b = (v - D) * x[k].x + (m - P) * x[k].y;
			if (b > w) return;
			e.m_pointCount = 1,
			e.m_type = d.collision.B2Manifold.e_faceA,
			e.m_localPlaneNormal.x = x[k].x,
			e.m_localPlaneNormal.y = x[k].y,
			e.m_localPlaneNormal.normalize(),
			e.m_localPoint.set(D, P),
			e.m_points[0].m_localPoint.setV(r.m_p),
			e.m_points[0].m_id.set_key(0)
		}
	},
	d.collision.B2Collision.testOverlap = function (e, t) {
		var n = t.lowerBound,
		r = e.upperBound,
		i = n.x - r.x,
		s = n.y - r.y;
		n = e.lowerBound,
		r = t.upperBound;
		var o = n.x - r.x,
		u = n.y - r.y;
		return i > 0 || s > 0 ? !1 : o > 0 || u > 0 ? !1 : !0
	},
	d.collision.B2ContactPoint = function () {
		this.position = new d.common.math.B2Vec2,
		this.velocity = new d.common.math.B2Vec2,
		this.normal = new d.common.math.B2Vec2,
		this.id = new d.collision.B2ContactID
	},
	d.collision.B2ContactPoint.__name__ = !0,
	d.collision.B2ContactPoint.prototype = {
		__class__: d.collision.B2ContactPoint
	},
	d.collision.B2Simplex = function () {
		this.m_v1 = new d.collision.B2SimplexVertex,
		this.m_v2 = new d.collision.B2SimplexVertex,
		this.m_v3 = new d.collision.B2SimplexVertex,
		this.m_vertices = new Array,
		this.m_vertices[0] = this.m_v1,
		this.m_vertices[1] = this.m_v2,
		this.m_vertices[2] = this.m_v3
	},
	d.collision.B2Simplex.__name__ = !0,
	d.collision.B2Simplex.prototype = {
		solve3: function () {
			var e = this.m_v1.w,
			t = this.m_v2.w,
			n = this.m_v3.w,
			r = d.common.math.B2Math.subtractVV(t, e),
			i = d.common.math.B2Math.dot(e, r),
			s = d.common.math.B2Math.dot(t, r),
			o = s,
			u = -i,
			a = d.common.math.B2Math.subtractVV(n, e),
			f = d.common.math.B2Math.dot(e, a),
			l = d.common.math.B2Math.dot(n, a),
			c = l,
			h = -f,
			p = d.common.math.B2Math.subtractVV(n, t),
			v = d.common.math.B2Math.dot(t, p),
			m = d.common.math.B2Math.dot(n, p),
			g = m,
			y = -v,
			b = d.common.math.B2Math.crossVV(r, a),
			w = b * d.common.math.B2Math.crossVV(t, n),
			E = b * d.common.math.B2Math.crossVV(n, e),
			S = b * d.common.math.B2Math.crossVV(e, t);
			if (u <= 0 && h <= 0) {
				this.m_v1.a = 1,
				this.m_count = 1;
				return
			}
			if (o > 0 && u > 0 && S <= 0) {
				var x = 1 / (o + u);
				this.m_v1.a = o * x,
				this.m_v2.a = u * x,
				this.m_count = 2;
				return
			}
			if (c > 0 && h > 0 && E <= 0) {
				var T = 1 / (c + h);
				this.m_v1.a = c * T,
				this.m_v3.a = h * T,
				this.m_count = 2,
				this.m_v2.set(this.m_v3);
				return
			}
			if (o <= 0 && y <= 0) {
				this.m_v2.a = 1,
				this.m_count = 1,
				this.m_v1.set(this.m_v2);
				return
			}
			if (c <= 0 && g <= 0) {
				this.m_v3.a = 1,
				this.m_count = 1,
				this.m_v1.set(this.m_v3);
				return
			}
			if (g > 0 && y > 0 && w <= 0) {
				var N = 1 / (g + y);
				this.m_v2.a = g * N,
				this.m_v3.a = y * N,
				this.m_count = 2,
				this.m_v1.set(this.m_v3);
				return
			}
			var C = 1 / (w + E + S);
			this.m_v1.a = w * C,
			this.m_v2.a = E * C,
			this.m_v3.a = S * C,
			this.m_count = 3
		},
		solve2: function () {
			var e = this.m_v1.w,
			t = this.m_v2.w,
			n = d.common.math.B2Math.subtractVV(t, e),
			r = -(e.x * n.x + e.y * n.y);
			if (r <= 0) {
				this.m_v1.a = 1,
				this.m_count = 1;
				return
			}
			var i = t.x * n.x + t.y * n.y;
			if (i <= 0) {
				this.m_v2.a = 1,
				this.m_count = 1,
				this.m_v1.set(this.m_v2);
				return
			}
			var s = 1 / (i + r);
			this.m_v1.a = i * s,
			this.m_v2.a = r * s,
			this.m_count = 2
		},
		getMetric: function () {
			var e = this;
			switch (e.m_count) {
			case 0:
				return d.common.B2Settings.b2Assert(!1),
				0;
			case 1:
				return 0;
			case 2:
				return d.common.math.B2Math.subtractVV(this.m_v1.w, this.m_v2.w).length();
			case 3:
				return d.common.math.B2Math.crossVV(d.common.math.B2Math.subtractVV(this.m_v2.w, this.m_v1.w), d.common.math.B2Math.subtractVV(this.m_v3.w, this.m_v1.w));
			default:
				return d.common.B2Settings.b2Assert(!1),
				0
			}
		},
		getWitnessPoints: function (e, t) {
			var n = this;
			switch (n.m_count) {
			case 0:
				d.common.B2Settings.b2Assert(!1);
				break;
			case 1:
				e.setV(this.m_v1.wA),
				t.setV(this.m_v1.wB);
				break;
			case 2:
				e.x = this.m_v1.a * this.m_v1.wA.x + this.m_v2.a * this.m_v2.wA.x,
				e.y = this.m_v1.a * this.m_v1.wA.y + this.m_v2.a * this.m_v2.wA.y,
				t.x = this.m_v1.a * this.m_v1.wB.x + this.m_v2.a * this.m_v2.wB.x,
				t.y = this.m_v1.a * this.m_v1.wB.y + this.m_v2.a * this.m_v2.wB.y;
				break;
			case 3:
				t.x = e.x = this.m_v1.a * this.m_v1.wA.x + this.m_v2.a * this.m_v2.wA.x + this.m_v3.a * this.m_v3.wA.x,
				t.y = e.y = this.m_v1.a * this.m_v1.wA.y + this.m_v2.a * this.m_v2.wA.y + this.m_v3.a * this.m_v3.wA.y;
				break;
			default:
				d.common.B2Settings.b2Assert(!1)
			}
		},
		getClosestPoint: function () {
			var e = this;
			switch (e.m_count) {
			case 0:
				return d.common.B2Settings.b2Assert(!1),
				new d.common.math.B2Vec2;
			case 1:
				return this.m_v1.w;
			case 2:
				return new d.common.math.B2Vec2(this.m_v1.a * this.m_v1.w.x + this.m_v2.a * this.m_v2.w.x, this.m_v1.a * this.m_v1.w.y + this.m_v2.a * this.m_v2.w.y);
			default:
				return d.common.B2Settings.b2Assert(!1),
				new d.common.math.B2Vec2
			}
		},
		getSearchDirection: function () {
			var e = this;
			switch (e.m_count) {
			case 1:
				return this.m_v1.w.getNegative();
			case 2:
				var t = d.common.math.B2Math.subtractVV(this.m_v2.w, this.m_v1.w),
				n = d.common.math.B2Math.crossVV(t, this.m_v1.w.getNegative());
				return n > 0 ? d.common.math.B2Math.crossFV(1, t) : d.common.math.B2Math.crossVF(t, 1);
			default:
				return d.common.B2Settings.b2Assert(!1),
				new d.common.math.B2Vec2
			}
		},
		writeCache: function (e) {
			e.metric = this.getMetric(),
			e.count = this.m_count | 0;
			var t = this.m_vertices,
			n = 0,
			r = this.m_count;
			while (n < r) {
				var i = n++;
				e.indexA[i] = t[i].indexA | 0,
				e.indexB[i] = t[i].indexB | 0
			}
		},
		readCache: function (e, t, n, r, i) {
			d.common.B2Settings.b2Assert(0 <= e.count && e.count <= 3);
			var s, o;
			this.m_count = e.count;
			var u = this.m_vertices,
			a, f = 0,
			l = this.m_count;
			while (f < l) {
				var c = f++;
				a = u[c],
				a.indexA = e.indexA[c],
				a.indexB = e.indexB[c],
				s = t.getVertex(a.indexA),
				o = r.getVertex(a.indexB),
				a.wA = d.common.math.B2Math.mulX(n, s),
				a.wB = d.common.math.B2Math.mulX(i, o),
				a.w = d.common.math.B2Math.subtractVV(a.wB, a.wA),
				a.a = 0
			}
			if (this.m_count > 1) {
				var h = e.metric,
				p = this.getMetric();
				if (p < .5 * h || 2 * h < p || p < 2.2250738585072014e-308) this.m_count = 0
			}
			this.m_count == 0 && (a = u[0], a.indexA = 0, a.indexB = 0, s = t.getVertex(0), o = r.getVertex(0), a.wA = d.common.math.B2Math.mulX(n, s), a.wB = d.common.math.B2Math.mulX(i, o), a.w = d.common.math.B2Math.subtractVV(a.wB, a.wA), this.m_count = 1)
		},
		__class__: d.collision.B2Simplex
	},
	d.collision.B2SimplexVertex = function () {},
	d.collision.B2SimplexVertex.__name__ = !0,
	d.collision.B2SimplexVertex.prototype = {
		set: function (e) {
			this.wA.setV(e.wA),
			this.wB.setV(e.wB),
			this.w.setV(e.w),
			this.a = e.a,
			this.indexA = e.indexA,
			this.indexB = e.indexB
		},
		__class__: d.collision.B2SimplexVertex
	},
	d.collision.B2Distance = function () {},
	d.collision.B2Distance.__name__ = !0,
	d.collision.B2Distance.distance = function (e, t, n) {++d.collision.B2Distance.b2_gjkCalls;
		var r = n.proxyA,
		i = n.proxyB,
		s = n.transformA,
		o = n.transformB,
		u = d.collision.B2Distance.s_simplex;
		u.readCache(t, r, s, i, o);
		var a = u.m_vertices,
		f = 20,
		l = d.collision.B2Distance.s_saveA,
		c = d.collision.B2Distance.s_saveB,
		h = 0,
		p = u.getClosestPoint(),
		v = p.lengthSquared(),
		m = v,
		g,
		y,
		b = 0;
		while (b < f) {
			h = u.m_count;
			var w = 0;
			while (w < h) {
				var E = w++;
				l[E] = a[E].indexA,
				c[E] = a[E].indexB
			}
			switch (u.m_count) {
			case 1:
				break;
			case 2:
				u.solve2();
				break;
			case 3:
				u.solve3();
				break;
			default:
				d.common.B2Settings.b2Assert(!1)
			}
			if (u.m_count == 3) break;
			y = u.getClosestPoint(),
			m = y.lengthSquared(),
			m > v,
			v = m;
			var S = u.getSearchDirection();
			if (S.lengthSquared() < 0) break;
			var x = a[u.m_count];
			x.indexA = r.getSupport(d.common.math.B2Math.mulTMV(s.R, S.getNegative())) | 0,
			x.wA = d.common.math.B2Math.mulX(s, r.getVertex(x.indexA)),
			x.indexB = i.getSupport(d.common.math.B2Math.mulTMV(o.R, S)) | 0,
			x.wB = d.common.math.B2Math.mulX(o, i.getVertex(x.indexB)),
			x.w = d.common.math.B2Math.subtractVV(x.wB, x.wA),
			++b,
			++d.collision.B2Distance.b2_gjkIters;
			var T = !1,
			w = 0;
			while (w < h) {
				var E = w++;
				if (x.indexA == l[E] && x.indexB == c[E]) {
					T = !0;
					break
				}
			}
			if (T) break; ++u.m_count
		}
		d.collision.B2Distance.b2_gjkMaxIters = d.common.math.B2Math.max(d.collision.B2Distance.b2_gjkMaxIters, b) | 0,
		u.getWitnessPoints(e.pointA, e.pointB),
		e.distance = d.common.math.B2Math.subtractVV(e.pointA, e.pointB).length(),
		e.iterations = b,
		u.writeCache(t);
		if (n.useRadii) {
			var N = r.m_radius,
			C = i.m_radius;
			if (e.distance > N + C && e.distance > 2.2250738585072014e-308) {
				e.distance -= N + C;
				var k = d.common.math.B2Math.subtractVV(e.pointB, e.pointA);
				k.normalize(),
				e.pointA.x += N * k.x,
				e.pointA.y += N * k.y,
				e.pointB.x -= C * k.x,
				e.pointB.y -= C * k.y
			} else y = new d.common.math.B2Vec2,
			y.x = .5 * (e.pointA.x + e.pointB.x),
			y.y = .5 * (e.pointA.y + e.pointB.y),
			e.pointA.x = e.pointB.x = y.x,
			e.pointA.y = e.pointB.y = y.y,
			e.distance = 0
		}
	},
	d.collision.B2DistanceInput = function () {},
	d.collision.B2DistanceInput.__name__ = !0,
	d.collision.B2DistanceInput.prototype = {
		__class__: d.collision.B2DistanceInput
	},
	d.collision.B2DistanceOutput = function () {
		this.pointA = new d.common.math.B2Vec2,
		this.pointB = new d.common.math.B2Vec2
	},
	d.collision.B2DistanceOutput.__name__ = !0,
	d.collision.B2DistanceOutput.prototype = {
		__class__: d.collision.B2DistanceOutput
	},
	d.collision.B2DistanceProxy = function () {
		this.m_vertices = new Array
	},
	d.collision.B2DistanceProxy.__name__ = !0,
	d.collision.B2DistanceProxy.prototype = {
		getVertex: function (e) {
			return d.common.B2Settings.b2Assert(0 <= e && e < this.m_count),
			this.m_vertices[e]
		},
		getVertexCount: function () {
			return this.m_count
		},
		getSupportVertex: function (e) {
			var t = 0,
			n = this.m_vertices[0].x * e.x + this.m_vertices[0].y * e.y,
			r = 1,
			i = this.m_count;
			while (r < i) {
				var s = r++,
				o = this.m_vertices[s].x * e.x + this.m_vertices[s].y * e.y;
				o > n && (t = s, n = o)
			}
			return this.m_vertices[t]
		},
		getSupport: function (e) {
			var t = 0,
			n = this.m_vertices[0].x * e.x + this.m_vertices[0].y * e.y,
			r = 1,
			i = this.m_count;
			while (r < i) {
				var s = r++,
				o = this.m_vertices[s].x * e.x + this.m_vertices[s].y * e.y;
				o > n && (t = s, n = o)
			}
			return t
		},
		set: function (e) {
			var t = e.getType();
			switch (t) {
			case d.collision.shapes.B2Shape.e_circleShape:
				var n = y.Boot.__cast(e, d.collision.shapes.B2CircleShape);
				this.m_vertices = new Array,
				this.m_vertices[0] = n.m_p,
				this.m_count = 1,
				this.m_radius = n.m_radius;
				break;
			case d.collision.shapes.B2Shape.e_polygonShape:
				var r = y.Boot.__cast(e, d.collision.shapes.B2PolygonShape);
				this.m_vertices = r.m_vertices,
				this.m_count = r.m_vertexCount,
				this.m_radius = r.m_radius;
				break;
			default:
				d.common.B2Settings.b2Assert(!1)
			}
		},
		__class__: d.collision.B2DistanceProxy
	},
	d.collision.B2DynamicTree = function () {
		this.m_root = null,
		this.m_freeList = null,
		this.m_path = 0,
		this.m_insertionCount = 0
	},
	d.collision.B2DynamicTree.__name__ = !0,
	d.collision.B2DynamicTree.prototype = {
		removeLeaf: function (e) {
			if (e == this.m_root) {
				this.m_root = null;
				return
			}
			var t = e.parent,
			n = t.parent,
			r;
			t.child1 == e ? r = t.child2: r = t.child1;
			if (n != null) {
				n.child1 == t ? n.child1 = r: n.child2 = r,
				r.parent = n,
				this.freeNode(t);
				while (n != null) {
					var i = n.aabb;
					n.aabb = new d.collision.B2AABB,
					n.aabb.combine(n.child1.aabb, n.child2.aabb);
					if (i.contains(n.aabb)) break;
					n = n.parent
				}
			} else this.m_root = r,
			r.parent = null,
			this.freeNode(t)
		},
		insertLeaf: function (e) {++this.m_insertionCount;
			if (this.m_root == null) {
				this.m_root = e,
				this.m_root.parent = null;
				return
			}
			var t = e.aabb.getCenter(),
			n = this.m_root;
			if (n.isLeaf() == 0) do {
				var r = n.child1,
				i = n.child2,
				s = Math.abs((r.aabb.lowerBound.x + r.aabb.upperBound.x) / 2 - t.x) + Math.abs((r.aabb.lowerBound.y + r.aabb.upperBound.y) / 2 - t.y), o = Math.abs((i.aabb.lowerBound.x + i.aabb.upperBound.x) / 2 - t.x) + Math.abs((i.aabb.lowerBound.y + i.aabb.upperBound.y) / 2 - t.y);
				s < o ? n = r: n = i
			} while (n.isLeaf() == 0);
			var u = n.parent,
			a = this.allocateNode();
			a.parent = u,
			a.userData = null,
			a.aabb.combine(e.aabb, n.aabb);
			if (u != null) {
				n.parent.child1 == n ? u.child1 = a: u.child2 = a,
				a.child1 = n,
				a.child2 = e,
				n.parent = a,
				e.parent = a;
				do {
					if (u.aabb.contains(a.aabb)) break;
					u.aabb.combine(u.child1.aabb, u.child2.aabb), a = u, u = u.parent
				} while (u != null)
			} else a.child1 = n,
			a.child2 = e,
			n.parent = a,
			e.parent = a,
			this.m_root = a
		},
		freeNode: function (e) {
			e.parent = this.m_freeList,
			this.m_freeList = e
		},
		allocateNode: function () {
			if (this.m_freeList != null) {
				var e = this.m_freeList;
				return this.m_freeList = e.parent,
				e.parent = null,
				e.child1 = null,
				e.child2 = null,
				e
			}
			return new d.collision.B2DynamicTreeNode
		},
		rayCast: function (e, t) {
			if (this.m_root == null) return;
			var n = t.p1,
			r = t.p2,
			i = d.common.math.B2Math.subtractVV(n, r);
			i.normalize();
			var s = d.common.math.B2Math.crossFV(1, i),
			o = d.common.math.B2Math.absV(s),
			u = t.maxFraction,
			a = new d.collision.B2AABB,
			f,
			l;
			f = n.x + u * (r.x - n.x),
			l = n.y + u * (r.y - n.y),
			a.lowerBound.x = Math.min(n.x, f),
			a.lowerBound.y = Math.min(n.y, l),
			a.upperBound.x = Math.max(n.x, f),
			a.upperBound.y = Math.max(n.y, l);
			var c = new Array,
			h = 0;
			c[h++] = this.m_root;
			while (h > 0) {
				var p = c[--h];
				if (p.aabb.testOverlap(a) == 0) continue;
				var v = p.aabb.getCenter(),
				m = p.aabb.getExtents(),
				g = Math.abs(s.x * (n.x - v.x) + s.y * (n.y - v.y)) - o.x * m.x - o.y * m.y;
				if (g > 0) continue;
				if (p.isLeaf()) {
					var y = new d.collision.B2RayCastInput;
					y.p1 = t.p1,
					y.p2 = t.p2,
					y.maxFraction = t.maxFraction,
					u = e(y, p);
					if (u == 0) return;
					f = n.x + u * (r.x - n.x),
					l = n.y + u * (r.y - n.y),
					a.lowerBound.x = Math.min(n.x, f),
					a.lowerBound.y = Math.min(n.y, l),
					a.upperBound.x = Math.max(n.x, f),
					a.upperBound.y = Math.max(n.y, l)
				} else c[h++] = p.child1,
				c[h++] = p.child2
			}
		},
		query: function (e, t) {
			if (this.m_root == null) return;
			var n = new Array,
			r = 0;
			n[r++] = this.m_root;
			while (r > 0) {
				var i = n[--r];
				if (i.aabb.testOverlap(t)) if (i.isLeaf()) {
					var s = e(i);
					if (!s) return
				} else n[r++] = i.child1,
				n[r++] = i.child2
			}
		},
		getUserData: function (e) {
			return e.userData
		},
		getFatAABB: function (e) {
			return e.aabb
		},
		rebalance: function (e) {
			if (this.m_root == null) return;
			var t = 0;
			while (t < e) {
				var n = t++,
				r = this.m_root,
				i = 0;
				while (r.isLeaf() == 0) r = (this.m_path >> i & 1) != 0 ? r.child2: r.child1,
				i = i + 1 & 31; ++this.m_path,
				this.removeLeaf(r),
				this.insertLeaf(r)
			}
		},
		moveProxy: function (e, t, n) {
			d.common.B2Settings.b2Assert(e.isLeaf());
			if (e.aabb.contains(t)) return ! 1;
			this.removeLeaf(e);
			var r = d.common.B2Settings.b2_aabbExtension + d.common.B2Settings.b2_aabbMultiplier * (n.x > 0 ? n.x: -n.x),
			i = d.common.B2Settings.b2_aabbExtension + d.common.B2Settings.b2_aabbMultiplier * (n.y > 0 ? n.y: -n.y);
			return e.aabb.lowerBound.x = t.lowerBound.x - r,
			e.aabb.lowerBound.y = t.lowerBound.y - i,
			e.aabb.upperBound.x = t.upperBound.x + r,
			e.aabb.upperBound.y = t.upperBound.y + i,
			this.insertLeaf(e),
			!0
		},
		destroyProxy: function (e) {
			this.removeLeaf(e),
			this.freeNode(e)
		},
		createProxy: function (e, t) {
			var n = this.allocateNode(),
			r = d.common.B2Settings.b2_aabbExtension,
			i = d.common.B2Settings.b2_aabbExtension;
			return n.aabb.lowerBound.x = e.lowerBound.x - r,
			n.aabb.lowerBound.y = e.lowerBound.y - i,
			n.aabb.upperBound.x = e.upperBound.x + r,
			n.aabb.upperBound.y = e.upperBound.y + i,
			n.userData = t,
			this.insertLeaf(n),
			n
		},
		__class__: d.collision.B2DynamicTree
	},
	d.collision.IBroadPhase = function () {},
	d.collision.IBroadPhase.__name__ = !0,
	d.collision.IBroadPhase.prototype = {
		__class__: d.collision.IBroadPhase
	},
	d.collision.B2DynamicTreeBroadPhase = function () {
		this.m_tree = new d.collision.B2DynamicTree,
		this.m_moveBuffer = new Array,
		this.m_pairBuffer = new Array,
		this.m_pairCount = 0
	},
	d.collision.B2DynamicTreeBroadPhase.__name__ = !0,
	d.collision.B2DynamicTreeBroadPhase.__interfaces__ = [d.collision.IBroadPhase],
	d.collision.B2DynamicTreeBroadPhase.prototype = {
		comparePairs: function (e, t) {
			return 0
		},
		unBufferMove: function (e) {
			i.remove(this.m_moveBuffer, e)
		},
		bufferMove: function (e) {
			this.m_moveBuffer[this.m_moveBuffer.length] = e
		},
		rebalance: function (e) {
			this.m_tree.rebalance(e)
		},
		validate: function () {},
		rayCast: function (e, t) {
			this.m_tree.rayCast(e, t)
		},
		query: function (e, t) {
			this.m_tree.query(e, t)
		},
		updatePairs: function (e) {
			var t = this;
			this.m_pairCount = 0;
			var n = 0,
			r = this.m_moveBuffer;
			while (n < r.length) {
				var i = [r[n]]; ++n;
				var s = function (e) {
					return function (n) {
						if (n == e[0]) return ! 0;
						t.m_pairCount == t.m_pairBuffer.length && (t.m_pairBuffer[t.m_pairCount] = new d.collision.B2DynamicTreePair);
						var r = t.m_pairBuffer[t.m_pairCount];
						return n.id < e[0].id ? (r.proxyA = n, r.proxyB = e[0]) : (r.proxyA = e[0], r.proxyB = n),
						++t.m_pairCount,
						!0
					}
				} (i),
				o = this.m_tree.getFatAABB(i[0]);
				this.m_tree.query(s, o)
			}
			this.m_moveBuffer = new Array;
			var u = !0,
			a = 0;
			while (u) if (a >= this.m_pairCount) u = !1;
			else {
				var f = this.m_pairBuffer[a],
				l = this.m_tree.getUserData(f.proxyA),
				c = this.m_tree.getUserData(f.proxyB);
				e(l, c),
				++a;
				while (a < this.m_pairCount) {
					var h = this.m_pairBuffer[a];
					if (h.proxyA != f.proxyA || h.proxyB != f.proxyB) break; ++a
				}
			}
		},
		getProxyCount: function () {
			return this.m_proxyCount
		},
		getFatAABB: function (e) {
			return this.m_tree.getFatAABB(e)
		},
		getUserData: function (e) {
			return this.m_tree.getUserData(e)
		},
		testOverlap: function (e, t) {
			var n = this.m_tree.getFatAABB(e),
			r = this.m_tree.getFatAABB(t);
			return n.testOverlap(r)
		},
		moveProxy: function (e, t, n) {
			var r = this.m_tree.moveProxy(e, t, n);
			r && this.bufferMove(e)
		},
		destroyProxy: function (e) {
			this.unBufferMove(e),
			--this.m_proxyCount,
			this.m_tree.destroyProxy(e)
		},
		createProxy: function (e, t) {
			var n = this.m_tree.createProxy(e, t);
			return++this.m_proxyCount,
			this.bufferMove(n),
			n
		},
		__class__: d.collision.B2DynamicTreeBroadPhase
	},
	d.collision.B2DynamicTreeNode = function () {
		this.aabb = new d.collision.B2AABB,
		this.id = d.collision.B2DynamicTreeNode.currentID++
	},
	d.collision.B2DynamicTreeNode.__name__ = !0,
	d.collision.B2DynamicTreeNode.prototype = {
		isLeaf: function () {
			return this.child1 == null
		},
		__class__: d.collision.B2DynamicTreeNode
	},
	d.collision.B2DynamicTreePair = function () {},
	d.collision.B2DynamicTreePair.__name__ = !0,
	d.collision.B2DynamicTreePair.prototype = {
		__class__: d.collision.B2DynamicTreePair
	},
	d.collision.B2Manifold = function () {
		this.m_pointCount = 0,
		this.m_points = new Array;
		var e = 0,
		t = d.common.B2Settings.b2_maxManifoldPoints;
		while (e < t) {
			var n = e++;
			this.m_points[n] = new d.collision.B2ManifoldPoint
		}
		this.m_localPlaneNormal = new d.common.math.B2Vec2,
		this.m_localPoint = new d.common.math.B2Vec2
	},
	d.collision.B2Manifold.__name__ = !0,
	d.collision.B2Manifold.prototype = {
		copy: function () {
			var e = new d.collision.B2Manifold;
			return e.set(this),
			e
		},
		set: function (e) {
			this.m_pointCount = e.m_pointCount;
			var t = 0,
			n = d.common.B2Settings.b2_maxManifoldPoints;
			while (t < n) {
				var r = t++;
				this.m_points[r].set(e.m_points[r])
			}
			this.m_localPlaneNormal.setV(e.m_localPlaneNormal),
			this.m_localPoint.setV(e.m_localPoint),
			this.m_type = e.m_type
		},
		reset: function () {
			var e = 0,
			t = d.common.B2Settings.b2_maxManifoldPoints;
			while (e < t) {
				var n = e++;
				this.m_points[n].reset()
			}
			this.m_localPlaneNormal.setZero(),
			this.m_localPoint.setZero(),
			this.m_type = 0,
			this.m_pointCount = 0
		},
		__class__: d.collision.B2Manifold
	},
	d.collision.B2ManifoldPoint = function () {
		this.m_localPoint = new d.common.math.B2Vec2,
		this.m_id = new d.collision.B2ContactID,
		this.reset()
	},
	d.collision.B2ManifoldPoint.__name__ = !0,
	d.collision.B2ManifoldPoint.prototype = {
		set: function (e) {
			this.m_localPoint.setV(e.m_localPoint),
			this.m_normalImpulse = e.m_normalImpulse,
			this.m_tangentImpulse = e.m_tangentImpulse,
			this.m_id.set(e.m_id)
		},
		reset: function () {
			this.m_localPoint.setZero(),
			this.m_normalImpulse = 0,
			this.m_tangentImpulse = 0,
			this.m_id.set_key(0)
		},
		__class__: d.collision.B2ManifoldPoint
	},
	d.collision.B2OBB = function () {
		this.R = new d.common.math.B2Mat22,
		this.center = new d.common.math.B2Vec2,
		this.extents = new d.common.math.B2Vec2
	},
	d.collision.B2OBB.__name__ = !0,
	d.collision.B2OBB.prototype = {
		__class__: d.collision.B2OBB
	},
	d.collision.B2RayCastInput = function (e, t, n) {
		n == null && (n = 1),
		this.p1 = new d.common.math.B2Vec2,
		this.p2 = new d.common.math.B2Vec2,
		e != null && this.p1.setV(e),
		t != null && this.p2.setV(t),
		this.maxFraction = n
	},
	d.collision.B2RayCastInput.__name__ = !0,
	d.collision.B2RayCastInput.prototype = {
		__class__: d.collision.B2RayCastInput
	},
	d.collision.B2RayCastOutput = function () {
		this.normal = new d.common.math.B2Vec2
	},
	d.collision.B2RayCastOutput.__name__ = !0,
	d.collision.B2RayCastOutput.prototype = {
		__class__: d.collision.B2RayCastOutput
	},
	d.collision.B2SeparationFunction = function () {
		this.m_localPoint = new d.common.math.B2Vec2,
		this.m_axis = new d.common.math.B2Vec2
	},
	d.collision.B2SeparationFunction.__name__ = !0,
	d.collision.B2SeparationFunction.prototype = {
		evaluate: function (e, t) {
			var n, r, i, s, o, u, a, f;
			return this.m_type == d.collision.B2SeparationFunction.e_points ? (n = d.common.math.B2Math.mulTMV(e.R, this.m_axis), r = d.common.math.B2Math.mulTMV(t.R, this.m_axis.getNegative()), i = this.m_proxyA.getSupportVertex(n), s = this.m_proxyB.getSupportVertex(r), o = d.common.math.B2Math.mulX(e, i), u = d.common.math.B2Math.mulX(t, s), a = (u.x - o.x) * this.m_axis.x + (u.y - o.y) * this.m_axis.y, a) : this.m_type == d.collision.B2SeparationFunction.e_faceA ? (f = d.common.math.B2Math.mulMV(e.R, this.m_axis), o = d.common.math.B2Math.mulX(e, this.m_localPoint), r = d.common.math.B2Math.mulTMV(t.R, f.getNegative()), s = this.m_proxyB.getSupportVertex(r), u = d.common.math.B2Math.mulX(t, s), a = (u.x - o.x) * f.x + (u.y - o.y) * f.y, a) : this.m_type == d.collision.B2SeparationFunction.e_faceB ? (f = d.common.math.B2Math.mulMV(t.R, this.m_axis), u = d.common.math.B2Math.mulX(t, this.m_localPoint), n = d.common.math.B2Math.mulTMV(e.R, f.getNegative()), i = this.m_proxyA.getSupportVertex(n), o = d.common.math.B2Math.mulX(e, i), a = (o.x - u.x) * f.x + (o.y - u.y) * f.y, a) : 0
		},
		initialize: function (e, t, n, r, i) {
			this.m_proxyA = t,
			this.m_proxyB = r;
			var s = e.count;
			d.common.B2Settings.b2Assert(0 < s && s < 3);
			var o = new d.common.math.B2Vec2,
			u, a, f = new d.common.math.B2Vec2,
			l, c, h, p, v, m, g, y, b, w, E, S;
			if (s == 1) this.m_type = d.collision.B2SeparationFunction.e_points,
			o = this.m_proxyA.getVertex(e.indexA[0]),
			f = this.m_proxyB.getVertex(e.indexB[0]),
			w = o,
			b = n.R,
			h = n.position.x + (b.col1.x * w.x + b.col2.x * w.y),
			p = n.position.y + (b.col1.y * w.x + b.col2.y * w.y),
			w = f,
			b = i.R,
			v = i.position.x + (b.col1.x * w.x + b.col2.x * w.y),
			m = i.position.y + (b.col1.y * w.x + b.col2.y * w.y),
			this.m_axis.x = v - h,
			this.m_axis.y = m - p,
			this.m_axis.normalize();
			else if (e.indexB[0] == e.indexB[1]) this.m_type = d.collision.B2SeparationFunction.e_faceA,
			u = this.m_proxyA.getVertex(e.indexA[0]),
			a = this.m_proxyA.getVertex(e.indexA[1]),
			f = this.m_proxyB.getVertex(e.indexB[0]),
			this.m_localPoint.x = .5 * (u.x + a.x),
			this.m_localPoint.y = .5 * (u.y + a.y),
			this.m_axis = d.common.math.B2Math.crossVF(d.common.math.B2Math.subtractVV(a, u), 1),
			this.m_axis.normalize(),
			w = this.m_axis,
			b = n.R,
			g = b.col1.x * w.x + b.col2.x * w.y,
			y = b.col1.y * w.x + b.col2.y * w.y,
			w = this.m_localPoint,
			b = n.R,
			h = n.position.x + (b.col1.x * w.x + b.col2.x * w.y),
			p = n.position.y + (b.col1.y * w.x + b.col2.y * w.y),
			w = f,
			b = i.R,
			v = i.position.x + (b.col1.x * w.x + b.col2.x * w.y),
			m = i.position.y + (b.col1.y * w.x + b.col2.y * w.y),
			E = (v - h) * g + (m - p) * y,
			E < 0 && this.m_axis.negativeSelf();
			else if (e.indexA[0] == e.indexA[0]) this.m_type = d.collision.B2SeparationFunction.e_faceB,
			l = this.m_proxyB.getVertex(e.indexB[0]),
			c = this.m_proxyB.getVertex(e.indexB[1]),
			o = this.m_proxyA.getVertex(e.indexA[0]),
			this.m_localPoint.x = .5 * (l.x + c.x),
			this.m_localPoint.y = .5 * (l.y + c.y),
			this.m_axis = d.common.math.B2Math.crossVF(d.common.math.B2Math.subtractVV(c, l), 1),
			this.m_axis.normalize(),
			w = this.m_axis,
			b = i.R,
			g = b.col1.x * w.x + b.col2.x * w.y,
			y = b.col1.y * w.x + b.col2.y * w.y,
			w = this.m_localPoint,
			b = i.R,
			v = i.position.x + (b.col1.x * w.x + b.col2.x * w.y),
			m = i.position.y + (b.col1.y * w.x + b.col2.y * w.y),
			w = o,
			b = n.R,
			h = n.position.x + (b.col1.x * w.x + b.col2.x * w.y),
			p = n.position.y + (b.col1.y * w.x + b.col2.y * w.y),
			E = (h - v) * g + (p - m) * y,
			E < 0 && this.m_axis.negativeSelf();
			else {
				u = this.m_proxyA.getVertex(e.indexA[0]),
				a = this.m_proxyA.getVertex(e.indexA[1]),
				l = this.m_proxyB.getVertex(e.indexB[0]),
				c = this.m_proxyB.getVertex(e.indexB[1]);
				var x = d.common.math.B2Math.mulX(n, o),
				T = d.common.math.B2Math.mulMV(n.R, d.common.math.B2Math.subtractVV(a, u)),
				N = d.common.math.B2Math.mulX(i, f),
				C = d.common.math.B2Math.mulMV(i.R, d.common.math.B2Math.subtractVV(c, l)),
				k = T.x * T.x + T.y * T.y,
				L = C.x * C.x + C.y * C.y,
				A = d.common.math.B2Math.subtractVV(C, T),
				O = T.x * A.x + T.y * A.y,
				M = C.x * A.x + C.y * A.y,
				_ = T.x * C.x + T.y * C.y,
				D = k * L - _ * _;
				E = 0,
				D != 0 && (E = d.common.math.B2Math.clamp((_ * M - O * L) / D, 0, 1));
				var P = (_ * E + M) / L;
				P < 0 && (P = 0, E = d.common.math.B2Math.clamp((_ - O) / k, 0, 1)),
				o = new d.common.math.B2Vec2,
				o.x = u.x + E * (a.x - u.x),
				o.y = u.y + E * (a.y - u.y),
				f = new d.common.math.B2Vec2,
				f.x = l.x + E * (c.x - l.x),
				f.y = l.y + E * (c.y - l.y),
				E == 0 || E == 1 ? (this.m_type = d.collision.B2SeparationFunction.e_faceB, this.m_axis = d.common.math.B2Math.crossVF(d.common.math.B2Math.subtractVV(c, l), 1), this.m_axis.normalize(), this.m_localPoint = f, w = this.m_axis, b = i.R, g = b.col1.x * w.x + b.col2.x * w.y, y = b.col1.y * w.x + b.col2.y * w.y, w = this.m_localPoint, b = i.R, v = i.position.x + (b.col1.x * w.x + b.col2.x * w.y), m = i.position.y + (b.col1.y * w.x + b.col2.y * w.y), w = o, b = n.R, h = n.position.x + (b.col1.x * w.x + b.col2.x * w.y), p = n.position.y + (b.col1.y * w.x + b.col2.y * w.y), S = (h - v) * g + (p - m) * y, E < 0 && this.m_axis.negativeSelf()) : (this.m_type = d.collision.B2SeparationFunction.e_faceA, this.m_axis = d.common.math.B2Math.crossVF(d.common.math.B2Math.subtractVV(a, u), 1), this.m_localPoint = o, w = this.m_axis, b = n.R, g = b.col1.x * w.x + b.col2.x * w.y, y = b.col1.y * w.x + b.col2.y * w.y, w = this.m_localPoint, b = n.R, h = n.position.x + (b.col1.x * w.x + b.col2.x * w.y), p = n.position.y + (b.col1.y * w.x + b.col2.y * w.y), w = f, b = i.R, v = i.position.x + (b.col1.x * w.x + b.col2.x * w.y), m = i.position.y + (b.col1.y * w.x + b.col2.y * w.y), S = (v - h) * g + (m - p) * y, E < 0 && this.m_axis.negativeSelf())
			}
		},
		__class__: d.collision.B2SeparationFunction
	},
	d.collision.B2SimplexCache = function () {
		this.indexA = new Array,
		this.indexB = new Array
	},
	d.collision.B2SimplexCache.__name__ = !0,
	d.collision.B2SimplexCache.prototype = {
		__class__: d.collision.B2SimplexCache
	},
	d.collision.B2TOIInput = function () {
		this.proxyA = new d.collision.B2DistanceProxy,
		this.proxyB = new d.collision.B2DistanceProxy,
		this.sweepA = new d.common.math.B2Sweep,
		this.sweepB = new d.common.math.B2Sweep
	},
	d.collision.B2TOIInput.__name__ = !0,
	d.collision.B2TOIInput.prototype = {
		__class__: d.collision.B2TOIInput
	},
	d.common.math.B2Transform = function (e, t) {
		this.position = new d.common.math.B2Vec2,
		this.R = new d.common.math.B2Mat22,
		e != null && (this.position.setV(e), this.R.setM(t))
	},
	d.common.math.B2Transform.__name__ = !0,
	d.common.math.B2Transform.prototype = {
		getAngle: function () {
			return Math.atan2(this.R.col1.y, this.R.col1.x)
		},
		set: function (e) {
			this.position.setV(e.position),
			this.R.setM(e.R)
		},
		setIdentity: function () {
			this.position.setZero(),
			this.R.setIdentity()
		},
		initialize: function (e, t) {
			this.position.setV(e),
			this.R.setM(t)
		},
		__class__: d.common.math.B2Transform
	},
	d.common.math.B2Mat22 = function () {
		this.col1 = new d.common.math.B2Vec2(0, 1),
		this.col2 = new d.common.math.B2Vec2(0, 1)
	},
	d.common.math.B2Mat22.__name__ = !0,
	d.common.math.B2Mat22.fromAngle = function (e) {
		var t = new d.common.math.B2Mat22;
		return t.set(e),
		t
	},
	d.common.math.B2Mat22.fromVV = function (e, t) {
		var n = new d.common.math.B2Mat22;
		return n.setVV(e, t),
		n
	},
	d.common.math.B2Mat22.prototype = {
		abs: function () {
			this.col1.abs(),
			this.col2.abs()
		},
		solve: function (e, t, n) {
			var r = this.col1.x,
			i = this.col2.x,
			s = this.col1.y,
			o = this.col2.y,
			u = r * o - i * s;
			return u != 0 && (u = 1 / u),
			e.x = u * (o * t - i * n),
			e.y = u * (r * n - s * t),
			e
		},
		getInverse: function (e) {
			var t = this.col1.x,
			n = this.col2.x,
			r = this.col1.y,
			i = this.col2.y,
			s = t * i - n * r;
			return s != 0 && (s = 1 / s),
			e.col1.x = s * i,
			e.col2.x = -s * n,
			e.col1.y = -s * r,
			e.col2.y = s * t,
			e
		},
		getAngle: function () {
			return Math.atan2(this.col1.y, this.col1.x)
		},
		setZero: function () {
			this.col1.x = 0,
			this.col2.x = 0,
			this.col1.y = 0,
			this.col2.y = 0
		},
		setIdentity: function () {
			this.col1.x = 1,
			this.col2.x = 0,
			this.col1.y = 0,
			this.col2.y = 1
		},
		addM: function (e) {
			this.col1.x += e.col1.x,
			this.col1.y += e.col1.y,
			this.col2.x += e.col2.x,
			this.col2.y += e.col2.y
		},
		setM: function (e) {
			this.col1.setV(e.col1),
			this.col2.setV(e.col2)
		},
		copy: function () {
			var e = new d.common.math.B2Mat22;
			return e.setM(this),
			e
		},
		setVV: function (e, t) {
			this.col1.setV(e),
			this.col2.setV(t)
		},
		set: function (e) {
			var t = Math.cos(e),
			n = Math.sin(e);
			this.col1.x = t,
			this.col2.x = -n,
			this.col1.y = n,
			this.col2.y = t
		},
		__class__: d.common.math.B2Mat22
	},
	d.collision.B2TimeOfImpact = function () {},
	d.collision.B2TimeOfImpact.__name__ = !0,
	d.collision.B2TimeOfImpact.timeOfImpact = function (e) {++d.collision.B2TimeOfImpact.b2_toiCalls;
		var t = e.proxyA,
		n = e.proxyB,
		r = e.sweepA,
		i = e.sweepB;
		d.common.B2Settings.b2Assert(r.t0 == i.t0),
		d.common.B2Settings.b2Assert(1 - r.t0 > 2.2250738585072014e-308);
		var s = t.m_radius + n.m_radius,
		o = e.tolerance,
		u = 0,
		a = 1e3,
		f = 0,
		l = 0;
		d.collision.B2TimeOfImpact.s_cache.count = 0,
		d.collision.B2TimeOfImpact.s_distanceInput.useRadii = !1;
		for (;;) {
			r.getTransform(d.collision.B2TimeOfImpact.s_xfA, u),
			i.getTransform(d.collision.B2TimeOfImpact.s_xfB, u),
			d.collision.B2TimeOfImpact.s_distanceInput.proxyA = t,
			d.collision.B2TimeOfImpact.s_distanceInput.proxyB = n,
			d.collision.B2TimeOfImpact.s_distanceInput.transformA = d.collision.B2TimeOfImpact.s_xfA,
			d.collision.B2TimeOfImpact.s_distanceInput.transformB = d.collision.B2TimeOfImpact.s_xfB,
			d.collision.B2Distance.distance(d.collision.B2TimeOfImpact.s_distanceOutput, d.collision.B2TimeOfImpact.s_cache, d.collision.B2TimeOfImpact.s_distanceInput);
			if (d.collision.B2TimeOfImpact.s_distanceOutput.distance <= 0) {
				u = 1;
				break
			}
			d.collision.B2TimeOfImpact.s_fcn.initialize(d.collision.B2TimeOfImpact.s_cache, t, d.collision.B2TimeOfImpact.s_xfA, n, d.collision.B2TimeOfImpact.s_xfB);
			var c = d.collision.B2TimeOfImpact.s_fcn.evaluate(d.collision.B2TimeOfImpact.s_xfA, d.collision.B2TimeOfImpact.s_xfB);
			if (c <= 0) {
				u = 1;
				break
			}
			f == 0 && (c > s ? l = d.common.math.B2Math.max(s - o, .75 * s) : l = d.common.math.B2Math.max(c - o, .02 * s));
			if (c - l < .5 * o) {
				if (f == 0) {
					u = 1;
					break
				}
				break
			}
			var h = u,
			p = u,
			v = 1,
			m = c;
			r.getTransform(d.collision.B2TimeOfImpact.s_xfA, v),
			i.getTransform(d.collision.B2TimeOfImpact.s_xfB, v);
			var g = d.collision.B2TimeOfImpact.s_fcn.evaluate(d.collision.B2TimeOfImpact.s_xfA, d.collision.B2TimeOfImpact.s_xfB);
			if (g >= l) {
				u = 1;
				break
			}
			var y = 0;
			for (;;) {
				var b;
				(y & 1) != 0 ? b = p + (l - m) * (v - p) / (g - m) : b = .5 * (p + v),
				r.getTransform(d.collision.B2TimeOfImpact.s_xfA, b),
				i.getTransform(d.collision.B2TimeOfImpact.s_xfB, b);
				var w = d.collision.B2TimeOfImpact.s_fcn.evaluate(d.collision.B2TimeOfImpact.s_xfA, d.collision.B2TimeOfImpact.s_xfB);
				if (d.common.math.B2Math.abs(w - l) < .025 * o) {
					h = b;
					break
				}
				w > l ? (p = b, m = w) : (v = b, g = w),
				++y,
				++d.collision.B2TimeOfImpact.b2_toiRootIters;
				if (y == 50) break
			}
			d.collision.B2TimeOfImpact.b2_toiMaxRootIters = d.common.math.B2Math.max(d.collision.B2TimeOfImpact.b2_toiMaxRootIters, y) | 0;
			if (h < 1 * u) break;
			u = h,
			f++,
			++d.collision.B2TimeOfImpact.b2_toiIters;
			if (f == a) break
		}
		return d.collision.B2TimeOfImpact.b2_toiMaxIters = d.common.math.B2Math.max(d.collision.B2TimeOfImpact.b2_toiMaxIters, f) | 0,
		u
	},
	d.collision.B2WorldManifold = function () {
		this.m_normal = new d.common.math.B2Vec2,
		this.m_points = new Array;
		var e = 0,
		t = d.common.B2Settings.b2_maxManifoldPoints;
		while (e < t) {
			var n = e++;
			this.m_points[n] = new d.common.math.B2Vec2
		}
	},
	d.collision.B2WorldManifold.__name__ = !0,
	d.collision.B2WorldManifold.prototype = {
		initialize: function (e, t, n, r, i) {
			if (e.m_pointCount == 0) return;
			var s, o, u, a, f, l, c, h, p;
			switch (e.m_type) {
			case d.collision.B2Manifold.e_circles:
				u = t.R,
				o = e.m_localPoint;
				var v = t.position.x + u.col1.x * o.x + u.col2.x * o.y,
				m = t.position.y + u.col1.y * o.x + u.col2.y * o.y;
				u = r.R,
				o = e.m_points[0].m_localPoint;
				var g = r.position.x + u.col1.x * o.x + u.col2.x * o.y,
				y = r.position.y + u.col1.y * o.x + u.col2.y * o.y,
				b = g - v,
				w = y - m,
				E = b * b + w * w;
				if (E > 0) {
					var S = Math.sqrt(E);
					this.m_normal.x = b / S,
					this.m_normal.y = w / S
				} else this.m_normal.x = 1,
				this.m_normal.y = 0;
				var x = v + n * this.m_normal.x,
				T = m + n * this.m_normal.y,
				N = g - i * this.m_normal.x,
				C = y - i * this.m_normal.y;
				this.m_points[0].x = .5 * (x + N),
				this.m_points[0].y = .5 * (T + C);
				break;
			case d.collision.B2Manifold.e_faceA:
				u = t.R,
				o = e.m_localPlaneNormal,
				a = u.col1.x * o.x + u.col2.x * o.y,
				f = u.col1.y * o.x + u.col2.y * o.y,
				u = t.R,
				o = e.m_localPoint,
				l = t.position.x + u.col1.x * o.x + u.col2.x * o.y,
				c = t.position.y + u.col1.y * o.x + u.col2.y * o.y,
				this.m_normal.x = a,
				this.m_normal.y = f;
				var k = 0,
				L = e.m_pointCount;
				while (k < L) {
					var A = k++;
					u = r.R,
					o = e.m_points[A].m_localPoint,
					h = r.position.x + u.col1.x * o.x + u.col2.x * o.y,
					p = r.position.y + u.col1.y * o.x + u.col2.y * o.y,
					this.m_points[A].x = h + .5 * (n - (h - l) * a - (p - c) * f - i) * a,
					this.m_points[A].y = p + .5 * (n - (h - l) * a - (p - c) * f - i) * f
				}
				break;
			case d.collision.B2Manifold.e_faceB:
				u = r.R,
				o = e.m_localPlaneNormal,
				a = u.col1.x * o.x + u.col2.x * o.y,
				f = u.col1.y * o.x + u.col2.y * o.y,
				u = r.R,
				o = e.m_localPoint,
				l = r.position.x + u.col1.x * o.x + u.col2.x * o.y,
				c = r.position.y + u.col1.y * o.x + u.col2.y * o.y,
				this.m_normal.x = -a,
				this.m_normal.y = -f;
				var k = 0,
				L = e.m_pointCount;
				while (k < L) {
					var A = k++;
					u = t.R,
					o = e.m_points[A].m_localPoint,
					h = t.position.x + u.col1.x * o.x + u.col2.x * o.y,
					p = t.position.y + u.col1.y * o.x + u.col2.y * o.y,
					this.m_points[A].x = h + .5 * (i - (h - l) * a - (p - c) * f - n) * a,
					this.m_points[A].y = p + .5 * (i - (h - l) * a - (p - c) * f - n) * f
				}
			}
		},
		__class__: d.collision.B2WorldManifold
	},
	d.collision.shapes = {},
	d.collision.shapes.B2Shape = function () {
		this.m_type = d.collision.shapes.B2Shape.e_unknownShape,
		this.m_radius = d.common.B2Settings.b2_linearSlop
	},
	d.collision.shapes.B2Shape.__name__ = !0,
	d.collision.shapes.B2Shape.testOverlap = function (e, t, n, r) {
		var i = new d.collision.B2DistanceInput;
		i.proxyA = new d.collision.B2DistanceProxy,
		i.proxyA.set(e),
		i.proxyB = new d.collision.B2DistanceProxy,
		i.proxyB.set(n),
		i.transformA = t,
		i.transformB = r,
		i.useRadii = !0;
		var s = new d.collision.B2SimplexCache;
		s.count = 0;
		var o = new d.collision.B2DistanceOutput;
		return d.collision.B2Distance.distance(o, s, i),
		o.distance < 2.2250738585072014e-307
	},
	d.collision.shapes.B2Shape.prototype = {
		computeSubmergedArea: function (e, t, n, r) {
			return 0
		},
		computeMass: function (e, t) {},
		computeAABB: function (e, t) {},
		rayCast: function (e, t, n) {
			return ! 1
		},
		testPoint: function (e, t) {
			return ! 1
		},
		getType: function () {
			return this.m_type
		},
		set: function (e) {
			this.m_radius = e.m_radius
		},
		copy: function () {
			return null
		},
		__class__: d.collision.shapes.B2Shape
	},
	d.collision.shapes.B2CircleShape = function (e) {
		e == null && (e = 0),
		d.collision.shapes.B2Shape.call(this),
		this.m_p = new d.common.math.B2Vec2,
		this.m_type = d.collision.shapes.B2Shape.e_circleShape,
		this.m_radius = e
	},
	d.collision.shapes.B2CircleShape.__name__ = !0,
	d.collision.shapes.B2CircleShape.__super__ = d.collision.shapes.B2Shape,
	d.collision.shapes.B2CircleShape.prototype = t(d.collision.shapes.B2Shape.prototype, {
		setRadius: function (e) {
			this.m_radius = e
		},
		getRadius: function () {
			return this.m_radius
		},
		setLocalPosition: function (e) {
			this.m_p.setV(e)
		},
		getLocalPosition: function () {
			return this.m_p
		},
		computeSubmergedArea: function (e, t, n, r) {
			var i = d.common.math.B2Math.mulX(n, this.m_p),
			s = -(d.common.math.B2Math.dot(e, i) - t);
			if (s < -this.m_radius + 2.2250738585072014e-308) return 0;
			if (s > this.m_radius) return r.setV(i),
			Math.PI * this.m_radius * this.m_radius;
			var o = this.m_radius * this.m_radius,
			u = s * s,
			a = o * (Math.asin(s / this.m_radius) + Math.PI / 2) + s * Math.sqrt(o - u),
			f = -2 / 3 * Math.pow(o - u, 1.5) / a;
			return r.x = i.x + e.x * f,
			r.y = i.y + e.y * f,
			a
		},
		computeMass: function (e, t) {
			e.mass = t * d.common.B2Settings.b2_pi * this.m_radius * this.m_radius,
			e.center.setV(this.m_p),
			e.I = e.mass * (.5 * this.m_radius * this.m_radius + (this.m_p.x * this.m_p.x + this.m_p.y * this.m_p.y))
		},
		computeAABB: function (e, t) {
			var n = t.R,
			r = t.position.x + (n.col1.x * this.m_p.x + n.col2.x * this.m_p.y),
			i = t.position.y + (n.col1.y * this.m_p.x + n.col2.y * this.m_p.y);
			e.lowerBound.set(r - this.m_radius, i - this.m_radius),
			e.upperBound.set(r + this.m_radius, i + this.m_radius)
		},
		rayCast: function (e, t, n) {
			var r = n.R,
			i = n.position.x + (r.col1.x * this.m_p.x + r.col2.x * this.m_p.y),
			s = n.position.y + (r.col1.y * this.m_p.x + r.col2.y * this.m_p.y),
			o = t.p1.x - i,
			u = t.p1.y - s,
			a = o * o + u * u - this.m_radius * this.m_radius,
			f = t.p2.x - t.p1.x,
			l = t.p2.y - t.p1.y,
			c = o * f + u * l,
			h = f * f + l * l,
			p = c * c - h * a;
			if (p < 0 || h < 2.2250738585072014e-308) return ! 1;
			var d = -(c + Math.sqrt(p));
			return 0 <= d && d <= t.maxFraction * h ? (d /= h, e.fraction = d, e.normal.x = o + d * f, e.normal.y = u + d * l, e.normal.normalize(), !0) : !1
		},
		testPoint: function (e, t) {
			var n = e.R,
			r = e.position.x + (n.col1.x * this.m_p.x + n.col2.x * this.m_p.y),
			i = e.position.y + (n.col1.y * this.m_p.x + n.col2.y * this.m_p.y);
			return r = t.x - r,
			i = t.y - i,
			r * r + i * i <= this.m_radius * this.m_radius
		},
		set: function (e) {
			d.collision.shapes.B2Shape.prototype.set.call(this, e);
			if (y.Boot.__instanceof(e, d.collision.shapes.B2CircleShape)) {
				var t = y.Boot.__cast(e, d.collision.shapes.B2CircleShape);
				this.m_p.setV(t.m_p)
			}
		},
		copy: function () {
			var e = new d.collision.shapes.B2CircleShape;
			return e.set(this),
			e
		},
		__class__: d.collision.shapes.B2CircleShape
	}),
	d.collision.shapes.B2EdgeShape = function (e, t) {
		d.collision.shapes.B2Shape.call(this),
		this.s_supportVec = new d.common.math.B2Vec2,
		this.m_v1 = new d.common.math.B2Vec2,
		this.m_v2 = new d.common.math.B2Vec2,
		this.m_coreV1 = new d.common.math.B2Vec2,
		this.m_coreV2 = new d.common.math.B2Vec2,
		this.m_normal = new d.common.math.B2Vec2,
		this.m_direction = new d.common.math.B2Vec2,
		this.m_cornerDir1 = new d.common.math.B2Vec2,
		this.m_cornerDir2 = new d.common.math.B2Vec2,
		this.m_type = d.collision.shapes.B2Shape.e_edgeShape,
		this.m_prevEdge = null,
		this.m_nextEdge = null,
		this.m_v1 = e,
		this.m_v2 = t,
		this.m_direction.set(this.m_v2.x - this.m_v1.x, this.m_v2.y - this.m_v1.y),
		this.m_length = this.m_direction.normalize(),
		this.m_normal.set(this.m_direction.y, -this.m_direction.x),
		this.m_coreV1.set( - d.common.B2Settings.b2_toiSlop * (this.m_normal.x - this.m_direction.x) + this.m_v1.x, -d.common.B2Settings.b2_toiSlop * (this.m_normal.y - this.m_direction.y) + this.m_v1.y),
		this.m_coreV2.set( - d.common.B2Settings.b2_toiSlop * (this.m_normal.x + this.m_direction.x) + this.m_v2.x, -d.common.B2Settings.b2_toiSlop * (this.m_normal.y + this.m_direction.y) + this.m_v2.y),
		this.m_cornerDir1 = this.m_normal,
		this.m_cornerDir2.set( - this.m_normal.x, -this.m_normal.y)
	},
	d.collision.shapes.B2EdgeShape.__name__ = !0,
	d.collision.shapes.B2EdgeShape.__super__ = d.collision.shapes.B2Shape,
	d.collision.shapes.B2EdgeShape.prototype = t(d.collision.shapes.B2Shape.prototype, {
		setNextEdge: function (e, t, n, r) {
			this.m_nextEdge = e,
			this.m_coreV2 = t,
			this.m_cornerDir2 = n,
			this.m_cornerConvex2 = r
		},
		setPrevEdge: function (e, t, n, r) {
			this.m_prevEdge = e,
			this.m_coreV1 = t,
			this.m_cornerDir1 = n,
			this.m_cornerConvex1 = r
		},
		support: function (e, t, n) {
			var r = e.R,
			i = e.position.x + (r.col1.x * this.m_coreV1.x + r.col2.x * this.m_coreV1.y),
			s = e.position.y + (r.col1.y * this.m_coreV1.x + r.col2.y * this.m_coreV1.y),
			o = e.position.x + (r.col1.x * this.m_coreV2.x + r.col2.x * this.m_coreV2.y),
			u = e.position.y + (r.col1.y * this.m_coreV2.x + r.col2.y * this.m_coreV2.y);
			return i * t + s * n > o * t + u * n ? (this.s_supportVec.x = i, this.s_supportVec.y = s) : (this.s_supportVec.x = o, this.s_supportVec.y = u),
			this.s_supportVec
		},
		getPrevEdge: function () {
			return this.m_prevEdge
		},
		getNextEdge: function () {
			return this.m_nextEdge
		},
		getFirstVertex: function (e) {
			var t = e.R;
			return new d.common.math.B2Vec2(e.position.x + (t.col1.x * this.m_coreV1.x + t.col2.x * this.m_coreV1.y), e.position.y + (t.col1.y * this.m_coreV1.x + t.col2.y * this.m_coreV1.y))
		},
		corner2IsConvex: function () {
			return this.m_cornerConvex2
		},
		corner1IsConvex: function () {
			return this.m_cornerConvex1
		},
		getCorner2Vector: function () {
			return this.m_cornerDir2
		},
		getCorner1Vector: function () {
			return this.m_cornerDir1
		},
		getDirectionVector: function () {
			return this.m_direction
		},
		getNormalVector: function () {
			return this.m_normal
		},
		getCoreVertex2: function () {
			return this.m_coreV2
		},
		getCoreVertex1: function () {
			return this.m_coreV1
		},
		getVertex2: function () {
			return this.m_v2
		},
		getVertex1: function () {
			return this.m_v1
		},
		getLength: function () {
			return this.m_length
		},
		computeSubmergedArea: function (e, t, n, r) {
			var i = new d.common.math.B2Vec2(e.x * t, e.y * t),
			s = d.common.math.B2Math.mulX(n, this.m_v1),
			o = d.common.math.B2Math.mulX(n, this.m_v2),
			u = d.common.math.B2Math.dot(e, s) - t,
			a = d.common.math.B2Math.dot(e, o) - t;
			if (u > 0) {
				if (a > 0) return 0;
				s.x = -a / (u - a) * s.x + u / (u - a) * o.x,
				s.y = -a / (u - a) * s.y + u / (u - a) * o.y
			} else a > 0 && (o.x = -a / (u - a) * s.x + u / (u - a) * o.x, o.y = -a / (u - a) * s.y + u / (u - a) * o.y);
			return r.x = (i.x + s.x + o.x) / 3,
			r.y = (i.y + s.y + o.y) / 3,
			.5 * ((s.x - i.x) * (o.y - i.y) - (s.y - i.y) * (o.x - i.x))
		},
		computeMass: function (e, t) {
			e.mass = 0,
			e.center.setV(this.m_v1),
			e.I = 0
		},
		computeAABB: function (e, t) {
			var n = t.R,
			r = t.position.x + (n.col1.x * this.m_v1.x + n.col2.x * this.m_v1.y),
			i = t.position.y + (n.col1.y * this.m_v1.x + n.col2.y * this.m_v1.y),
			s = t.position.x + (n.col1.x * this.m_v2.x + n.col2.x * this.m_v2.y),
			o = t.position.y + (n.col1.y * this.m_v2.x + n.col2.y * this.m_v2.y);
			r < s ? (e.lowerBound.x = r, e.upperBound.x = s) : (e.lowerBound.x = s, e.upperBound.x = r),
			i < o ? (e.lowerBound.y = i, e.upperBound.y = o) : (e.lowerBound.y = o, e.upperBound.y = i)
		},
		rayCast: function (e, t, n) {
			var r, i = t.p2.x - t.p1.x,
			s = t.p2.y - t.p1.y;
			r = n.R;
			var o = n.position.x + (r.col1.x * this.m_v1.x + r.col2.x * this.m_v1.y),
			u = n.position.y + (r.col1.y * this.m_v1.x + r.col2.y * this.m_v1.y),
			a = n.position.y + (r.col1.y * this.m_v2.x + r.col2.y * this.m_v2.y) - u,
			f = -(n.position.x + (r.col1.x * this.m_v2.x + r.col2.x * this.m_v2.y) - o),
			l = 2.2250738585072014e-306,
			c = -(i * a + s * f);
			if (c > l) {
				var h = t.p1.x - o,
				p = t.p1.y - u,
				d = h * a + p * f;
				if (0 <= d && d <= t.maxFraction * c) {
					var v = -i * p + s * h;
					if ( - l * c <= v && v <= c * (1 + l)) {
						d /= c,
						e.fraction = d;
						var m = Math.sqrt(a * a + f * f);
						return e.normal.x = a / m,
						e.normal.y = f / m,
						!0
					}
				}
			}
			return ! 1
		},
		testPoint: function (e, t) {
			return ! 1
		},
		__class__: d.collision.shapes.B2EdgeShape
	}),
	d.collision.shapes.B2MassData = function () {
		this.mass = 0,
		this.center = new d.common.math.B2Vec2(0, 0),
		this.I = 0
	},
	d.collision.shapes.B2MassData.__name__ = !0,
	d.collision.shapes.B2MassData.prototype = {
		__class__: d.collision.shapes.B2MassData
	},
	d.collision.shapes.B2PolygonShape = function () {
		d.collision.shapes.B2Shape.call(this),
		this.m_type = d.collision.shapes.B2Shape.e_polygonShape,
		this.m_centroid = new d.common.math.B2Vec2,
		this.m_vertices = new Array,
		this.m_normals = new Array
	},
	d.collision.shapes.B2PolygonShape.__name__ = !0,
	d.collision.shapes.B2PolygonShape.asArray = function (e, t) {
		var n = new d.collision.shapes.B2PolygonShape;
		return n.setAsArray(e, t),
		n
	},
	d.collision.shapes.B2PolygonShape.asVector = function (e, t) {
		var n = new d.collision.shapes.B2PolygonShape;
		return n.setAsVector(e, t),
		n
	},
	d.collision.shapes.B2PolygonShape.asBox = function (e, t) {
		var n = new d.collision.shapes.B2PolygonShape;
		return n.setAsBox(e, t),
		n
	},
	d.collision.shapes.B2PolygonShape.asOrientedBox = function (e, t, n, r) {
		r == null && (r = 0);
		var i = new d.collision.shapes.B2PolygonShape;
		return i.setAsOrientedBox(e, t, n, r),
		i
	},
	d.collision.shapes.B2PolygonShape.asEdge = function (e, t) {
		var n = new d.collision.shapes.B2PolygonShape;
		return n.setAsEdge(e, t),
		n
	},
	d.collision.shapes.B2PolygonShape.computeCentroid = function (e, t) {
		var n = new d.common.math.B2Vec2,
		r = 0,
		i = 0,
		s = 0,
		o = 1 / 3,
		u = 0;
		while (u < t) {
			var a = u++,
			f = e[a],
			l = a + 1 < t ? e[a + 1 | 0] : e[0],
			c = f.x - i,
			h = f.y - s,
			p = l.x - i,
			v = l.y - s,
			m = c * v - h * p,
			g = .5 * m;
			r += g,
			n.x += g * o * (i + f.x + l.x),
			n.y += g * o * (s + f.y + l.y)
		}
		return n.x *= 1 / r,
		n.y *= 1 / r,
		n
	},
	d.collision.shapes.B2PolygonShape.computeOBB = function (e, t, n) {
		var r, i = new Array,
		s = 0;
		while (s < n) {
			var o = s++;
			i[o] = t[o]
		}
		i[n] = i[0];
		var u = 1.7976931348623157e308,
		a = 1,
		s = n + 1;
		while (a < s) {
			var o = a++,
			f = i[o - 1 | 0],
			l = i[o].x - f.x,
			c = i[o].y - f.y,
			h = Math.sqrt(l * l + c * c);
			l /= h,
			c /= h;
			var p = -c,
			d = l,
			v = 1.7976931348623157e308,
			m = 1.7976931348623157e308,
			g = -1.7976931348623157e308,
			y = -1.7976931348623157e308,
			b = 0;
			while (b < n) {
				var w = b++,
				E = i[w].x - f.x,
				S = i[w].y - f.y,
				x = l * E + c * S,
				T = p * E + d * S;
				x < v && (v = x),
				T < m && (m = T),
				x > g && (g = x),
				T > y && (y = T)
			}
			var N = (g - v) * (y - m);
			if (N < .95 * u) {
				u = N,
				e.R.col1.x = l,
				e.R.col1.y = c,
				e.R.col2.x = p,
				e.R.col2.y = d;
				var C = .5 * (v + g),
				k = .5 * (m + y),
				L = e.R;
				e.center.x = f.x + (L.col1.x * C + L.col2.x * k),
				e.center.y = f.y + (L.col1.y * C + L.col2.y * k),
				e.extents.x = .5 * (g - v),
				e.extents.y = .5 * (y - m)
			}
		}
	},
	d.collision.shapes.B2PolygonShape.__super__ = d.collision.shapes.B2Shape,
	d.collision.shapes.B2PolygonShape.prototype = t(d.collision.shapes.B2Shape.prototype, {
		reserve: function (e) {
			var t = this.m_vertices.length;
			while (t < e) {
				var n = t++;
				this.m_vertices[n] = new d.common.math.B2Vec2,
				this.m_normals[n] = new d.common.math.B2Vec2
			}
		},
		validate: function () {
			return ! 1
		},
		getSupportVertex: function (e) {
			var t = 0,
			n = this.m_vertices[0].x * e.x + this.m_vertices[0].y * e.y,
			r = 1,
			i = this.m_vertexCount;
			while (r < i) {
				var s = r++,
				o = this.m_vertices[s].x * e.x + this.m_vertices[s].y * e.y;
				o > n && (t = s, n = o)
			}
			return this.m_vertices[t]
		},
		getSupport: function (e) {
			var t = 0,
			n = this.m_vertices[0].x * e.x + this.m_vertices[0].y * e.y,
			r = 1,
			i = this.m_vertexCount;
			while (r < i) {
				var s = r++,
				o = this.m_vertices[s].x * e.x + this.m_vertices[s].y * e.y;
				o > n && (t = s, n = o)
			}
			return t
		},
		getNormals: function () {
			return this.m_normals
		},
		getVertices: function () {
			return this.m_vertices
		},
		getVertexCount: function () {
			return this.m_vertexCount
		},
		computeSubmergedArea: function (e, t, n, r) {
			var i = d.common.math.B2Math.mulTMV(n.R, e),
			s = t - d.common.math.B2Math.dot(e, n.position),
			o = new Array,
			u = 0,
			a = -1,
			f = -1,
			l = !1,
			c,
			h = 0,
			p = this.m_vertexCount;
			while (h < p) {
				var v = h++;
				o[v] = d.common.math.B2Math.dot(i, this.m_vertices[v]) - s;
				var m = o[v] < -2.2250738585072014e-308;
				v > 0 && (m ? l || (a = v - 1, u++) : l && (f = v - 1, u++)),
				l = m
			}
			switch (u) {
			case 0:
				if (l) {
					var g = new d.collision.shapes.B2MassData;
					return this.computeMass(g, 1),
					r.setV(d.common.math.B2Math.mulX(n, g.center)),
					g.mass
				}
				return 0;
			case 1:
				a == -1 ? a = this.m_vertexCount - 1 : f = this.m_vertexCount - 1
			}
			var y = (a + 1) % this.m_vertexCount,
			b = (f + 1) % this.m_vertexCount,
			w = (0 - o[a]) / (o[y] - o[a]),
			E = (0 - o[f]) / (o[b] - o[f]),
			S = new d.common.math.B2Vec2(this.m_vertices[a].x * (1 - w) + this.m_vertices[y].x * w, this.m_vertices[a].y * (1 - w) + this.m_vertices[y].y * w),
			x = new d.common.math.B2Vec2(this.m_vertices[f].x * (1 - E) + this.m_vertices[b].x * E, this.m_vertices[f].y * (1 - E) + this.m_vertices[b].y * E),
			T = 0,
			N = new d.common.math.B2Vec2,
			C = this.m_vertices[y],
			k;
			c = y;
			while (c != b) {
				c = (c + 1) % this.m_vertexCount,
				c == b ? k = x: k = this.m_vertices[c];
				var L = .5 * ((C.x - S.x) * (k.y - S.y) - (C.y - S.y) * (k.x - S.x));
				T += L,
				N.x += L * (S.x + C.x + k.x) / 3,
				N.y += L * (S.y + C.y + k.y) / 3,
				C = k
			}
			return N.multiply(1 / T),
			r.setV(d.common.math.B2Math.mulX(n, N)),
			T
		},
		computeMass: function (e, t) {
			if (this.m_vertexCount == 2) {
				e.center.x = .5 * (this.m_vertices[0].x + this.m_vertices[1].x),
				e.center.y = .5 * (this.m_vertices[0].y + this.m_vertices[1].y),
				e.mass = 0,
				e.I = 0;
				return
			}
			var n = 0,
			r = 0,
			i = 0,
			s = 0,
			o = 0,
			u = 0,
			a = 1 / 3,
			f = 0,
			l = this.m_vertexCount;
			while (f < l) {
				var c = f++,
				h = this.m_vertices[c],
				p = c + 1 < this.m_vertexCount ? this.m_vertices[c + 1 | 0] : this.m_vertices[0],
				d = h.x - o,
				v = h.y - u,
				m = p.x - o,
				g = p.y - u,
				y = d * g - v * m,
				b = .5 * y;
				i += b,
				n += b * a * (o + h.x + p.x),
				r += b * a * (u + h.y + p.y);
				var w = o,
				E = u,
				S = d,
				x = v,
				T = m,
				N = g,
				C = a * (.25 * (S * S + T * S + T * T) + (w * S + w * T)) + .5 * w * w,
				k = a * (.25 * (x * x + N * x + N * N) + (E * x + E * N)) + .5 * E * E;
				s += y * (C + k)
			}
			e.mass = t * i,
			n *= 1 / i,
			r *= 1 / i,
			e.center.set(n, r),
			e.I = t * s
		},
		computeAABB: function (e, t) {
			var n = t.R,
			r = this.m_vertices[0],
			i = t.position.x + (n.col1.x * r.x + n.col2.x * r.y),
			s = t.position.y + (n.col1.y * r.x + n.col2.y * r.y),
			o = i,
			u = s,
			a = 1,
			f = this.m_vertexCount;
			while (a < f) {
				var l = a++;
				r = this.m_vertices[l];
				var c = t.position.x + (n.col1.x * r.x + n.col2.x * r.y),
				h = t.position.y + (n.col1.y * r.x + n.col2.y * r.y);
				i = i < c ? i: c,
				s = s < h ? s: h,
				o = o > c ? o: c,
				u = u > h ? u: h
			}
			e.lowerBound.x = i - this.m_radius,
			e.lowerBound.y = s - this.m_radius,
			e.upperBound.x = o + this.m_radius,
			e.upperBound.y = u + this.m_radius
		},
		rayCast: function (e, t, n) {
			var r = 0,
			i = t.maxFraction,
			s, o, u, a;
			s = t.p1.x - n.position.x,
			o = t.p1.y - n.position.y,
			u = n.R;
			var f = s * u.col1.x + o * u.col1.y,
			l = s * u.col2.x + o * u.col2.y;
			s = t.p2.x - n.position.x,
			o = t.p2.y - n.position.y,
			u = n.R;
			var c = s * u.col1.x + o * u.col1.y,
			h = s * u.col2.x + o * u.col2.y,
			p = c - f,
			d = h - l,
			v = -1,
			m = 0,
			g = this.m_vertexCount;
			while (m < g) {
				var y = m++;
				a = this.m_vertices[y],
				s = a.x - f,
				o = a.y - l,
				a = this.m_normals[y];
				var b = a.x * s + a.y * o,
				w = a.x * p + a.y * d;
				if (w == 0) {
					if (b < 0) return ! 1
				} else w < 0 && b < r * w ? (r = b / w, v = y) : w > 0 && b < i * w && (i = b / w);
				if (i < r - 2.2250738585072014e-308) return ! 1
			}
			return v >= 0 ? (e.fraction = r, u = n.R, a = this.m_normals[v], e.normal.x = u.col1.x * a.x + u.col2.x * a.y, e.normal.y = u.col1.y * a.x + u.col2.y * a.y, !0) : !1
		},
		testPoint: function (e, t) {
			var n, r = e.R,
			i = t.x - e.position.x,
			s = t.y - e.position.y,
			o = i * r.col1.x + s * r.col1.y,
			u = i * r.col2.x + s * r.col2.y,
			a = 0,
			f = this.m_vertexCount;
			while (a < f) {
				var l = a++;
				n = this.m_vertices[l],
				i = o - n.x,
				s = u - n.y,
				n = this.m_normals[l];
				var c = n.x * i + n.y * s;
				if (c > 0) return ! 1
			}
			return ! 0
		},
		setAsEdge: function (e, t) {
			this.m_vertexCount = 2,
			this.reserve(2),
			this.m_vertices[0].setV(e),
			this.m_vertices[1].setV(t),
			this.m_centroid.x = .5 * (e.x + t.x),
			this.m_centroid.y = .5 * (e.y + t.y),
			this.m_normals[0] = d.common.math.B2Math.crossVF(d.common.math.B2Math.subtractVV(t, e), 1),
			this.m_normals[0].normalize(),
			this.m_normals[1].x = -this.m_normals[0].x,
			this.m_normals[1].y = -this.m_normals[0].y
		},
		setAsOrientedBox: function (e, t, n, r) {
			r == null && (r = 0),
			this.m_vertexCount = 4,
			this.reserve(4),
			this.m_vertices[0].set( - e, -t),
			this.m_vertices[1].set(e, -t),
			this.m_vertices[2].set(e, t),
			this.m_vertices[3].set( - e, t),
			this.m_normals[0].set(0, -1),
			this.m_normals[1].set(1, 0),
			this.m_normals[2].set(0, 1),
			this.m_normals[3].set( - 1, 0),
			this.m_centroid = n;
			var i = new d.common.math.B2Transform;
			i.position = n,
			i.R.set(r);
			var s = 0,
			o = this.m_vertexCount;
			while (s < o) {
				var u = s++;
				this.m_vertices[u] = d.common.math.B2Math.mulX(i, this.m_vertices[u]),
				this.m_normals[u] = d.common.math.B2Math.mulMV(i.R, this.m_normals[u])
			}
		},
		setAsBox: function (e, t) {
			this.m_vertexCount = 4,
			this.reserve(4),
			this.m_vertices[0].set( - e, -t),
			this.m_vertices[1].set(e, -t),
			this.m_vertices[2].set(e, t),
			this.m_vertices[3].set( - e, t),
			this.m_normals[0].set(0, -1),
			this.m_normals[1].set(1, 0),
			this.m_normals[2].set(0, 1),
			this.m_normals[3].set( - 1, 0),
			this.m_centroid.setZero()
		},
		setAsVector: function (e, t) {
			t == null && (t = 0),
			t == 0 && (t = e.length),
			d.common.B2Settings.b2Assert(2 <= t),
			this.m_vertexCount = t | 0,
			this.reserve(t | 0);
			var n, r = 0,
			i = this.m_vertexCount;
			while (r < i) {
				var s = r++;
				this.m_vertices[s].setV(e[s])
			}
			var r = 0,
			i = this.m_vertexCount;
			while (r < i) {
				var s = r++,
				o = s,
				u = s + 1 < this.m_vertexCount ? s + 1 : 0,
				a = d.common.math.B2Math.subtractVV(this.m_vertices[u], this.m_vertices[o]);
				d.common.B2Settings.b2Assert(a.lengthSquared() > 2.2250738585072014e-308),
				this.m_normals[s].setV(d.common.math.B2Math.crossVF(a, 1)),
				this.m_normals[s].normalize()
			}
			this.m_centroid = d.collision.shapes.B2PolygonShape.computeCentroid(this.m_vertices, this.m_vertexCount)
		},
		setAsArray: function (e, t) {
			t == null && (t = 0);
			var n = new Array,
			r = 0;
			while (r < e.length) {
				var i = e[r]; ++r,
				n.push(i)
			}
			this.setAsVector(n, t)
		},
		set: function (e) {
			d.collision.shapes.B2Shape.prototype.set.call(this, e);
			if (y.Boot.__instanceof(e, d.collision.shapes.B2PolygonShape)) {
				var t = y.Boot.__cast(e, d.collision.shapes.B2PolygonShape);
				this.m_centroid.setV(t.m_centroid),
				this.m_vertexCount = t.m_vertexCount,
				this.reserve(this.m_vertexCount);
				var n = 0,
				r = this.m_vertexCount;
				while (n < r) {
					var i = n++;
					this.m_vertices[i].setV(t.m_vertices[i]),
					this.m_normals[i].setV(t.m_normals[i])
				}
			}
		},
		copy: function () {
			var e = new d.collision.shapes.B2PolygonShape;
			return e.set(this),
			e
		},
		__class__: d.collision.shapes.B2PolygonShape
	}),
	d.common.B2Color = function (e, t, n) {
		this._r = 255 * d.common.math.B2Math.clamp(e, 0, 1) | 0,
		this._g = 255 * d.common.math.B2Math.clamp(t, 0, 1) | 0,
		this._b = 255 * d.common.math.B2Math.clamp(n, 0, 1) | 0
	},
	d.common.B2Color.__name__ = !0,
	d.common.B2Color.prototype = {
		get_color: function () {
			return this._r << 16 | this._g << 8 | this._b
		},
		set_b: function (e) {
			return this._b = 255 * d.common.math.B2Math.clamp(e, 0, 1) | 0
		},
		set_g: function (e) {
			return this._g = 255 * d.common.math.B2Math.clamp(e, 0, 1) | 0
		},
		set_r: function (e) {
			return this._r = 255 * d.common.math.B2Math.clamp(e, 0, 1) | 0
		},
		set: function (e, t, n) {
			this._r = 255 * d.common.math.B2Math.clamp(e, 0, 1) | 0,
			this._g = 255 * d.common.math.B2Math.clamp(t, 0, 1) | 0,
			this._b = 255 * d.common.math.B2Math.clamp(n, 0, 1) | 0
		},
		__class__: d.common.B2Color
	},
	d.common.B2Settings = function () {},
	d.common.B2Settings.__name__ = !0,
	d.common.B2Settings.b2MixFriction = function (e, t) {
		return Math.sqrt(e * t)
	},
	d.common.B2Settings.b2MixRestitution = function (e, t) {
		return e > t ? e: t
	},
	d.common.B2Settings.b2Assert = function (e) {
		if (!e) throw "Assertion Failed"
	},
	d.common.math.B2Mat33 = function (e, t, n) {
		this.col1 = new d.common.math.B2Vec3,
		this.col2 = new d.common.math.B2Vec3,
		this.col3 = new d.common.math.B2Vec3,
		e == null && t == null && n == null ? (this.col1.setZero(), this.col2.setZero(), this.col3.setZero()) : (this.col1.setV(e), this.col2.setV(t), this.col3.setV(n))
	},
	d.common.math.B2Mat33.__name__ = !0,
	d.common.math.B2Mat33.prototype = {
		solve33: function (e, t, n, r) {
			var i = this.col1.x,
			s = this.col1.y,
			o = this.col1.z,
			u = this.col2.x,
			a = this.col2.y,
			f = this.col2.z,
			l = this.col3.x,
			c = this.col3.y,
			h = this.col3.z,
			p = i * (a * h - f * c) + s * (f * l - u * h) + o * (u * c - a * l);
			return p != 0 && (p = 1 / p),
			e.x = p * (t * (a * h - f * c) + n * (f * l - u * h) + r * (u * c - a * l)),
			e.y = p * (i * (n * h - r * c) + s * (r * l - t * h) + o * (t * c - n * l)),
			e.z = p * (i * (a * r - f * n) + s * (f * t - u * r) + o * (u * n - a * t)),
			e
		},
		solve22: function (e, t, n) {
			var r = this.col1.x,
			i = this.col2.x,
			s = this.col1.y,
			o = this.col2.y,
			u = r * o - i * s;
			return u != 0 && (u = 1 / u),
			e.x = u * (o * t - i * n),
			e.y = u * (r * n - s * t),
			e
		},
		setZero: function () {
			this.col1.x = 0,
			this.col2.x = 0,
			this.col3.x = 0,
			this.col1.y = 0,
			this.col2.y = 0,
			this.col3.y = 0,
			this.col1.z = 0,
			this.col2.z = 0,
			this.col3.z = 0
		},
		setIdentity: function () {
			this.col1.x = 1,
			this.col2.x = 0,
			this.col3.x = 0,
			this.col1.y = 0,
			this.col2.y = 1,
			this.col3.y = 0,
			this.col1.z = 0,
			this.col2.z = 0,
			this.col3.z = 1
		},
		addM: function (e) {
			this.col1.x += e.col1.x,
			this.col1.y += e.col1.y,
			this.col1.z += e.col1.z,
			this.col2.x += e.col2.x,
			this.col2.y += e.col2.y,
			this.col2.z += e.col2.z,
			this.col3.x += e.col3.x,
			this.col3.y += e.col3.y,
			this.col3.z += e.col3.z
		},
		setM: function (e) {
			this.col1.setV(e.col1),
			this.col2.setV(e.col2),
			this.col3.setV(e.col3)
		},
		copy: function () {
			return new d.common.math.B2Mat33(this.col1, this.col2, this.col3)
		},
		setVVV: function (e, t, n) {
			this.col1.setV(e),
			this.col2.setV(t),
			this.col3.setV(n)
		},
		__class__: d.common.math.B2Mat33
	},
	d.common.math.B2Math = function () {},
	d.common.math.B2Math.__name__ = !0,
	d.common.math.B2Math.isValid = function (e) {
		return Math.isNaN(e) || e == Math.NEGATIVE_INFINITY || e == Math.POSITIVE_INFINITY ? !1 : !0
	},
	d.common.math.B2Math.dot = function (e, t) {
		return e.x * t.x + e.y * t.y
	},
	d.common.math.B2Math.crossVV = function (e, t) {
		return e.x * t.y - e.y * t.x
	},
	d.common.math.B2Math.crossVF = function (e, t) {
		var n = new d.common.math.B2Vec2(t * e.y, -t * e.x);
		return n
	},
	d.common.math.B2Math.crossFV = function (e, t) {
		var n = new d.common.math.B2Vec2( - e * t.y, e * t.x);
		return n
	},
	d.common.math.B2Math.mulMV = function (e, t) {
		var n = new d.common.math.B2Vec2(e.col1.x * t.x + e.col2.x * t.y, e.col1.y * t.x + e.col2.y * t.y);
		return n
	},
	d.common.math.B2Math.mulTMV = function (e, t) {
		var n = new d.common.math.B2Vec2(d.common.math.B2Math.dot(t, e.col1), d.common.math.B2Math.dot(t, e.col2));
		return n
	},
	d.common.math.B2Math.mulX = function (e, t) {
		var n = d.common.math.B2Math.mulMV(e.R, t);
		return n.x += e.position.x,
		n.y += e.position.y,
		n
	},
	d.common.math.B2Math.mulXT = function (e, t) {
		var n = d.common.math.B2Math.subtractVV(t, e.position),
		r = n.x * e.R.col1.x + n.y * e.R.col1.y;
		return n.y = n.x * e.R.col2.x + n.y * e.R.col2.y,
		n.x = r,
		n
	},
	d.common.math.B2Math.addVV = function (e, t) {
		var n = new d.common.math.B2Vec2(e.x + t.x, e.y + t.y);
		return n
	},
	d.common.math.B2Math.subtractVV = function (e, t) {
		var n = new d.common.math.B2Vec2(e.x - t.x, e.y - t.y);
		return n
	},
	d.common.math.B2Math.distance = function (e, t) {
		var n = e.x - t.x,
		r = e.y - t.y;
		return Math.sqrt(n * n + r * r)
	},
	d.common.math.B2Math.distanceSquared = function (e, t) {
		var n = e.x - t.x,
		r = e.y - t.y;
		return n * n + r * r
	},
	d.common.math.B2Math.mulFV = function (e, t) {
		var n = new d.common.math.B2Vec2(e * t.x, e * t.y);
		return n
	},
	d.common.math.B2Math.addMM = function (e, t) {
		var n = d.common.math.B2Mat22.fromVV(d.common.math.B2Math.addVV(e.col1, t.col1), d.common.math.B2Math.addVV(e.col2, t.col2));
		return n
	},
	d.common.math.B2Math.mulMM = function (e, t) {
		var n = d.common.math.B2Mat22.fromVV(d.common.math.B2Math.mulMV(e, t.col1), d.common.math.B2Math.mulMV(e, t.col2));
		return n
	},
	d.common.math.B2Math.mulTMM = function (e, t) {
		var n = new d.common.math.B2Vec2(d.common.math.B2Math.dot(e.col1, t.col1), d.common.math.B2Math.dot(e.col2, t.col1)),
		r = new d.common.math.B2Vec2(d.common.math.B2Math.dot(e.col1, t.col2), d.common.math.B2Math.dot(e.col2, t.col2)),
		i = d.common.math.B2Mat22.fromVV(n, r);
		return i
	},
	d.common.math.B2Math.abs = function (e) {
		return e > 0 ? e: -e
	},
	d.common.math.B2Math.absV = function (e) {
		var t = new d.common.math.B2Vec2(d.common.math.B2Math.abs(e.x), d.common.math.B2Math.abs(e.y));
		return t
	},
	d.common.math.B2Math.absM = function (e) {
		var t = d.common.math.B2Mat22.fromVV(d.common.math.B2Math.absV(e.col1), d.common.math.B2Math.absV(e.col2));
		return t
	},
	d.common.math.B2Math.min = function (e, t) {
		return e < t ? e: t
	},
	d.common.math.B2Math.minV = function (e, t) {
		var n = new d.common.math.B2Vec2(d.common.math.B2Math.min(e.x, t.x), d.common.math.B2Math.min(e.y, t.y));
		return n
	},
	d.common.math.B2Math.max = function (e, t) {
		return e > t ? e: t
	},
	d.common.math.B2Math.maxV = function (e, t) {
		var n = new d.common.math.B2Vec2(d.common.math.B2Math.max(e.x, t.x), d.common.math.B2Math.max(e.y, t.y));
		return n
	},
	d.common.math.B2Math.clamp = function (e, t, n) {
		return e < t ? t: e > n ? n: e
	},
	d.common.math.B2Math.clampV = function (e, t, n) {
		return d.common.math.B2Math.maxV(t, d.common.math.B2Math.minV(e, n))
	},
	d.common.math.B2Math.swap = function (e, t) {
		var n = e[0];
		e[0] = t[0],
		t[0] = n
	},
	d.common.math.B2Math.random = function () {
		return Math.random() * 2 - 1
	},
	d.common.math.B2Math.randomRange = function (e, t) {
		var n = Math.random();
		return n = (t - e) * n + e,
		n
	},
	d.common.math.B2Math.nextPowerOfTwo = function (e) {
		return e |= e >> 1 & 2147483647,
		e |= e >> 2 & 1073741823,
		e |= e >> 4 & 268435455,
		e |= e >> 8 & 16777215,
		e |= e >> 16 & 65535,
		e + 1
	},
	d.common.math.B2Math.isPowerOfTwo = function (e) {
		var t = e > 0 && (e & e - 1) == 0;
		return t
	},
	d.common.math.B2Sweep = function () {
		this.localCenter = new d.common.math.B2Vec2,
		this.c0 = new d.common.math.B2Vec2,
		this.c = new d.common.math.B2Vec2
	},
	d.common.math.B2Sweep.__name__ = !0,
	d.common.math.B2Sweep.prototype = {
		advance: function (e) {
			if (this.t0 < e && 1 - this.t0 > 2.2250738585072014e-308) {
				var t = (e - this.t0) / (1 - this.t0);
				this.c0.x = (1 - t) * this.c0.x + t * this.c.x,
				this.c0.y = (1 - t) * this.c0.y + t * this.c.y,
				this.a0 = (1 - t) * this.a0 + t * this.a,
				this.t0 = e
			}
		},
		getTransform: function (e, t) {
			e.position.x = (1 - t) * this.c0.x + t * this.c.x,
			e.position.y = (1 - t) * this.c0.y + t * this.c.y;
			var n = (1 - t) * this.a0 + t * this.a;
			e.R.set(n);
			var r = e.R;
			e.position.x -= r.col1.x * this.localCenter.x + r.col2.x * this.localCenter.y,
			e.position.y -= r.col1.y * this.localCenter.x + r.col2.y * this.localCenter.y
		},
		copy: function () {
			var e = new d.common.math.B2Sweep;
			return e.localCenter.setV(this.localCenter),
			e.c0.setV(this.c0),
			e.c.setV(this.c),
			e.a0 = this.a0,
			e.a = this.a,
			e.t0 = this.t0,
			e
		},
		set: function (e) {
			this.localCenter.setV(e.localCenter),
			this.c0.setV(e.c0),
			this.c.setV(e.c),
			this.a0 = e.a0,
			this.a = e.a,
			this.t0 = e.t0
		},
		__class__: d.common.math.B2Sweep
	},
	d.common.math.B2Vec3 = function (e, t, n) {
		n == null && (n = 0),
		t == null && (t = 0),
		e == null && (e = 0),
		this.x = e,
		this.y = t,
		this.z = n
	},
	d.common.math.B2Vec3.__name__ = !0,
	d.common.math.B2Vec3.prototype = {
		multiply: function (e) {
			this.x *= e,
			this.y *= e,
			this.z *= e
		},
		subtract: function (e) {
			this.x -= e.x,
			this.y -= e.y,
			this.z -= e.z
		},
		add: function (e) {
			this.x += e.x,
			this.y += e.y,
			this.z += e.z
		},
		copy: function () {
			return new d.common.math.B2Vec3(this.x, this.y, this.z)
		},
		negativeSelf: function () {
			this.x = -this.x,
			this.y = -this.y,
			this.z = -this.z
		},
		getNegative: function () {
			return new d.common.math.B2Vec3( - this.x, -this.y, -this.z)
		},
		setV: function (e) {
			this.x = e.x,
			this.y = e.y,
			this.z = e.z
		},
		set: function (e, t, n) {
			this.x = e,
			this.y = t,
			this.z = n
		},
		setZero: function () {
			this.x = this.y = this.z = 0
		},
		__class__: d.common.math.B2Vec3
	},
	d.dynamics = {},
	d.dynamics.B2Body = function (e, t) {
		this.m_xf = new d.common.math.B2Transform,
		this.m_sweep = new d.common.math.B2Sweep,
		this.m_linearVelocity = new d.common.math.B2Vec2,
		this.m_force = new d.common.math.B2Vec2,
		this.m_flags = 0,
		e.bullet && (this.m_flags |= d.dynamics.B2Body.e_bulletFlag),
		e.fixedRotation && (this.m_flags |= d.dynamics.B2Body.e_fixedRotationFlag),
		e.allowSleep && (this.m_flags |= d.dynamics.B2Body.e_allowSleepFlag),
		e.awake && (this.m_flags |= d.dynamics.B2Body.e_awakeFlag),
		e.active && (this.m_flags |= d.dynamics.B2Body.e_activeFlag),
		this.m_world = t,
		this.m_xf.position.setV(e.position),
		this.m_xf.R.set(e.angle),
		this.m_sweep.localCenter.setZero(),
		this.m_sweep.t0 = 1,
		this.m_sweep.a0 = this.m_sweep.a = e.angle;
		var n = this.m_xf.R,
		r = this.m_sweep.localCenter;
		this.m_sweep.c.x = n.col1.x * r.x + n.col2.x * r.y,
		this.m_sweep.c.y = n.col1.y * r.x + n.col2.y * r.y,
		this.m_sweep.c.x += this.m_xf.position.x,
		this.m_sweep.c.y += this.m_xf.position.y,
		this.m_sweep.c0.setV(this.m_sweep.c),
		this.m_jointList = null,
		this.m_controllerList = null,
		this.m_contactList = null,
		this.m_controllerCount = 0,
		this.m_prev = null,
		this.m_next = null,
		this.m_linearVelocity.setV(e.linearVelocity),
		this.m_angularVelocity = e.angularVelocity,
		this.m_linearDamping = e.linearDamping,
		this.m_angularDamping = e.angularDamping,
		this.m_force.set(0, 0),
		this.m_torque = 0,
		this.m_sleepTime = 0,
		this.m_type = e.type,
		this.m_type == d.dynamics.B2Body.b2_dynamicBody ? (this.m_mass = 1, this.m_invMass = 1) : (this.m_mass = 0, this.m_invMass = 0),
		this.m_I = 0,
		this.m_invI = 0,
		this.m_inertiaScale = e.inertiaScale,
		this.m_userData = e.userData,
		this.m_fixtureList = null,
		this.m_fixtureCount = 0
	},
	d.dynamics.B2Body.__name__ = !0,
	d.dynamics.B2Body.prototype = {
		advance: function (e) {
			this.m_sweep.advance(e),
			this.m_sweep.c.setV(this.m_sweep.c0),
			this.m_sweep.a = this.m_sweep.a0,
			this.synchronizeTransform()
		},
		shouldCollide: function (e) {
			if (this.m_type != d.dynamics.B2Body.b2_dynamicBody && e.m_type != d.dynamics.B2Body.b2_dynamicBody) return ! 1;
			var t = this.m_jointList;
			while (t != null) {
				if (t.other == e && t.joint.m_collideConnected == 0) return ! 1;
				t = t.next
			}
			return ! 0
		},
		synchronizeTransform: function () {
			this.m_xf.R.set(this.m_sweep.a);
			var e = this.m_xf.R,
			t = this.m_sweep.localCenter;
			this.m_xf.position.x = this.m_sweep.c.x - (e.col1.x * t.x + e.col2.x * t.y),
			this.m_xf.position.y = this.m_sweep.c.y - (e.col1.y * t.x + e.col2.y * t.y)
		},
		synchronizeFixtures: function () {
			var e = d.dynamics.B2Body.s_xf1;
			e.R.set(this.m_sweep.a0);
			var t = e.R,
			n = this.m_sweep.localCenter;
			e.position.x = this.m_sweep.c0.x - (t.col1.x * n.x + t.col2.x * n.y),
			e.position.y = this.m_sweep.c0.y - (t.col1.y * n.x + t.col2.y * n.y);
			var r, i = this.m_world.m_contactManager.m_broadPhase;
			r = this.m_fixtureList;
			while (r != null) r.synchronize(i, e, this.m_xf),
			r = r.m_next
		},
		getWorld: function () {
			return this.m_world
		},
		setUserData: function (e) {
			this.m_userData = e
		},
		getUserData: function () {
			return this.m_userData
		},
		getNext: function () {
			return this.m_next
		},
		getContactList: function () {
			return this.m_contactList
		},
		getControllerList: function () {
			return this.m_controllerList
		},
		getJointList: function () {
			return this.m_jointList
		},
		getFixtureList: function () {
			return this.m_fixtureList
		},
		isSleepingAllowed: function () {
			return (this.m_flags & d.dynamics.B2Body.e_allowSleepFlag) == d.dynamics.B2Body.e_allowSleepFlag
		},
		isActive: function () {
			return (this.m_flags & d.dynamics.B2Body.e_activeFlag) == d.dynamics.B2Body.e_activeFlag
		},
		setActive: function (e) {
			if (e == this.isActive()) return;
			var t, n;
			if (e) {
				this.m_flags |= d.dynamics.B2Body.e_activeFlag,
				t = this.m_world.m_contactManager.m_broadPhase,
				n = this.m_fixtureList;
				while (n != null) n.createProxy(t, this.m_xf),
				n = n.m_next
			} else {
				this.m_flags &= ~d.dynamics.B2Body.e_activeFlag,
				t = this.m_world.m_contactManager.m_broadPhase,
				n = this.m_fixtureList;
				while (n != null) n.destroyProxy(t),
				n = n.m_next;
				var r = this.m_contactList;
				while (r != null) {
					var i = r;
					r = r.next,
					this.m_world.m_contactManager.destroy(i.contact)
				}
				this.m_contactList = null
			}
		},
		isFixedRotation: function () {
			return (this.m_flags & d.dynamics.B2Body.e_fixedRotationFlag) == d.dynamics.B2Body.e_fixedRotationFlag
		},
		setFixedRotation: function (e) {
			e ? this.m_flags |= d.dynamics.B2Body.e_fixedRotationFlag: this.m_flags &= ~d.dynamics.B2Body.e_fixedRotationFlag,
			this.resetMassData()
		},
		isAwake: function () {
			return (this.m_flags & d.dynamics.B2Body.e_awakeFlag) == d.dynamics.B2Body.e_awakeFlag
		},
		setAwake: function (e) {
			e ? (this.m_flags |= d.dynamics.B2Body.e_awakeFlag, this.m_sleepTime = 0) : (this.m_flags &= ~d.dynamics.B2Body.e_awakeFlag, this.m_sleepTime = 0, this.m_linearVelocity.setZero(), this.m_angularVelocity = 0, this.m_force.setZero(), this.m_torque = 0)
		},
		setSleepingAllowed: function (e) {
			e ? this.m_flags |= d.dynamics.B2Body.e_allowSleepFlag: (this.m_flags &= ~d.dynamics.B2Body.e_allowSleepFlag, this.setAwake(!0))
		},
		isBullet: function () {
			return (this.m_flags & d.dynamics.B2Body.e_bulletFlag) == d.dynamics.B2Body.e_bulletFlag
		},
		setBullet: function (e) {
			e ? this.m_flags |= d.dynamics.B2Body.e_bulletFlag: this.m_flags &= ~d.dynamics.B2Body.e_bulletFlag
		},
		getType: function () {
			return this.m_type
		},
		setType: function (e) {
			if (this.m_type == e) return;
			this.m_type = e,
			this.resetMassData(),
			this.m_type == d.dynamics.B2Body.b2_staticBody && (this.m_linearVelocity.setZero(), this.m_angularVelocity = 0),
			this.setAwake(!0),
			this.m_force.setZero(),
			this.m_torque = 0;
			var t = this.m_contactList;
			while (t != null) t.contact.flagForFiltering(),
			t = t.next
		},
		setAngularDamping: function (e) {
			this.m_angularDamping = e
		},
		getAngularDamping: function () {
			return this.m_angularDamping
		},
		setLinearDamping: function (e) {
			this.m_linearDamping = e
		},
		getLinearDamping: function () {
			return this.m_linearDamping
		},
		getLinearVelocityFromLocalPoint: function (e) {
			var t = this.m_xf.R,
			n = new d.common.math.B2Vec2(t.col1.x * e.x + t.col2.x * e.y, t.col1.y * e.x + t.col2.y * e.y);
			return n.x += this.m_xf.position.x,
			n.y += this.m_xf.position.y,
			new d.common.math.B2Vec2(this.m_linearVelocity.x - this.m_angularVelocity * (n.y - this.m_sweep.c.y), this.m_linearVelocity.y + this.m_angularVelocity * (n.x - this.m_sweep.c.x))
		},
		getLinearVelocityFromWorldPoint: function (e) {
			return new d.common.math.B2Vec2(this.m_linearVelocity.x - this.m_angularVelocity * (e.y - this.m_sweep.c.y), this.m_linearVelocity.y + this.m_angularVelocity * (e.x - this.m_sweep.c.x))
		},
		getLocalVector: function (e) {
			return d.common.math.B2Math.mulTMV(this.m_xf.R, e)
		},
		getLocalPoint: function (e) {
			return d.common.math.B2Math.mulXT(this.m_xf, e)
		},
		getWorldVector: function (e) {
			return d.common.math.B2Math.mulMV(this.m_xf.R, e)
		},
		getWorldPoint: function (e) {
			var t = this.m_xf.R,
			n = new d.common.math.B2Vec2(t.col1.x * e.x + t.col2.x * e.y, t.col1.y * e.x + t.col2.y * e.y);
			return n.x += this.m_xf.position.x,
			n.y += this.m_xf.position.y,
			n
		},
		resetMassData: function () {
			this.m_mass = 0,
			this.m_invMass = 0,
			this.m_I = 0,
			this.m_invI = 0,
			this.m_sweep.localCenter.setZero();
			if (this.m_type == d.dynamics.B2Body.b2_staticBody || this.m_type == d.dynamics.B2Body.b2_kinematicBody) return;
			var e = d.common.math.B2Vec2.make(0, 0),
			t = this.m_fixtureList;
			while (t != null) {
				if (t.m_density == 0) continue;
				var n = t.getMassData();
				this.m_mass += n.mass,
				e.x += n.center.x * n.mass,
				e.y += n.center.y * n.mass,
				this.m_I += n.I,
				t = t.m_next
			}
			this.m_mass > 0 ? (this.m_invMass = 1 / this.m_mass, e.x *= this.
			m_invMass, e.y *= this.m_invMass) : (this.m_mass = 1, this.m_invMass = 1),
			this.m_I > 0 && (this.m_flags & d.dynamics.B2Body.e_fixedRotationFlag) == 0 ? (this.m_I -= this.m_mass * (e.x * e.x + e.y * e.y), this.m_I *= this.m_inertiaScale, d.common.B2Settings.b2Assert(this.m_I > 0), this.m_invI = 1 / this.m_I) : (this.m_I = 0, this.m_invI = 0);
			var r = this.m_sweep.c.copy();
			this.m_sweep.localCenter.setV(e),
			this.m_sweep.c0.setV(d.common.math.B2Math.mulX(this.m_xf, this.m_sweep.localCenter)),
			this.m_sweep.c.setV(this.m_sweep.c0),
			this.m_linearVelocity.x += this.m_angularVelocity * -(this.m_sweep.c.y - r.y),
			this.m_linearVelocity.y += this.m_angularVelocity * (this.m_sweep.c.x - r.x)
		},
		setMassData: function (e) {
			d.common.B2Settings.b2Assert(this.m_world.isLocked() == 0);
			if (this.m_world.isLocked() == 1) return;
			if (this.m_type != d.dynamics.B2Body.b2_dynamicBody) return;
			this.m_invMass = 0,
			this.m_I = 0,
			this.m_invI = 0,
			this.m_mass = e.mass,
			this.m_mass <= 0 && (this.m_mass = 1),
			this.m_invMass = 1 / this.m_mass,
			e.I > 0 && (this.m_flags & d.dynamics.B2Body.e_fixedRotationFlag) == 0 && (this.m_I = e.I - this.m_mass * (e.center.x * e.center.x + e.center.y * e.center.y), this.m_invI = 1 / this.m_I);
			var t = this.m_sweep.c.copy();
			this.m_sweep.localCenter.setV(e.center),
			this.m_sweep.c0.setV(d.common.math.B2Math.mulX(this.m_xf, this.m_sweep.localCenter)),
			this.m_sweep.c.setV(this.m_sweep.c0),
			this.m_linearVelocity.x += this.m_angularVelocity * -(this.m_sweep.c.y - t.y),
			this.m_linearVelocity.y += this.m_angularVelocity * (this.m_sweep.c.x - t.x)
		},
		getMassData: function (e) {
			e.mass = this.m_mass,
			e.I = this.m_I,
			e.center.setV(this.m_sweep.localCenter)
		},
		getInertia: function () {
			return this.m_I
		},
		getMass: function () {
			return this.m_mass
		},
		merge: function (e) {
			var t;
			t = e.m_fixtureList;
			var n = this,
			r = e;
			while (t != null) {
				var i = t.m_next;
				e.m_fixtureCount--,
				t.m_next = this.m_fixtureList,
				this.m_fixtureList = t,
				this.m_fixtureCount++,
				t.m_body = r,
				t = i
			}
			n.m_fixtureCount = 0;
			var s = n.getWorldCenter(),
			o = r.getWorldCenter(),
			u = n.getLinearVelocity().copy(),
			a = r.getLinearVelocity().copy(),
			f = n.getAngularVelocity(),
			l = r.getAngularVelocity();
			n.resetMassData(),
			this.synchronizeFixtures()
		},
		split: function (e) {
			var t = this.getLinearVelocity().copy(),
			n = this.getAngularVelocity(),
			r = this.getWorldCenter(),
			i = this,
			s = this.m_world.createBody(this.getDefinition()),
			o = null,
			u = i.m_fixtureList;
			while (u != null) if (e(u)) {
				var a = u.m_next;
				o != null ? o.m_next = a: i.m_fixtureList = a,
				i.m_fixtureCount--,
				u.m_next = s.m_fixtureList,
				s.m_fixtureList = u,
				s.m_fixtureCount++,
				u.m_body = s,
				u = a
			} else o = u,
			u = u.m_next;
			i.resetMassData(),
			s.resetMassData();
			var f = i.getWorldCenter(),
			l = s.getWorldCenter(),
			c = d.common.math.B2Math.addVV(t, d.common.math.B2Math.crossFV(n, d.common.math.B2Math.subtractVV(f, r))),
			h = d.common.math.B2Math.addVV(t, d.common.math.B2Math.crossFV(n, d.common.math.B2Math.subtractVV(l, r)));
			return i.setLinearVelocity(c),
			s.setLinearVelocity(h),
			i.setAngularVelocity(n),
			s.setAngularVelocity(n),
			i.synchronizeFixtures(),
			s.synchronizeFixtures(),
			s
		},
		applyImpulse: function (e, t) {
			if (this.m_type != d.dynamics.B2Body.b2_dynamicBody) return;
			this.isAwake() == 0 && this.setAwake(!0),
			this.m_linearVelocity.x += this.m_invMass * e.x,
			this.m_linearVelocity.y += this.m_invMass * e.y,
			this.m_angularVelocity += this.m_invI * ((t.x - this.m_sweep.c.x) * e.y - (t.y - this.m_sweep.c.y) * e.x)
		},
		applyTorque: function (e) {
			if (this.m_type != d.dynamics.B2Body.b2_dynamicBody) return;
			this.isAwake() == 0 && this.setAwake(!0),
			this.m_torque += e
		},
		applyForce: function (e, t) {
			if (this.m_type != d.dynamics.B2Body.b2_dynamicBody) return;
			this.isAwake() == 0 && this.setAwake(!0),
			this.m_force.x += e.x,
			this.m_force.y += e.y,
			this.m_torque += (t.x - this.m_sweep.c.x) * e.y - (t.y - this.m_sweep.c.y) * e.x
		},
		getDefinition: function () {
			var e = new d.dynamics.B2BodyDef;
			return e.type = this.getType(),
			e.allowSleep = (this.m_flags & d.dynamics.B2Body.e_allowSleepFlag) == d.dynamics.B2Body.e_allowSleepFlag,
			e.angle = this.getAngle(),
			e.angularDamping = this.m_angularDamping,
			e.angularVelocity = this.m_angularVelocity,
			e.fixedRotation = (this.m_flags & d.dynamics.B2Body.e_fixedRotationFlag) == d.dynamics.B2Body.e_fixedRotationFlag,
			e.bullet = (this.m_flags & d.dynamics.B2Body.e_bulletFlag) == d.dynamics.B2Body.e_bulletFlag,
			e.awake = (this.m_flags & d.dynamics.B2Body.e_awakeFlag) == d.dynamics.B2Body.e_awakeFlag,
			e.linearDamping = this.m_linearDamping,
			e.linearVelocity.setV(this.getLinearVelocity()),
			e.position = this.getPosition(),
			e.userData = this.getUserData(),
			e
		},
		getAngularVelocity: function () {
			return this.m_angularVelocity
		},
		setAngularVelocity: function (e) {
			if (this.m_type == d.dynamics.B2Body.b2_staticBody) return;
			this.m_angularVelocity = e
		},
		getLinearVelocity: function () {
			return this.m_linearVelocity
		},
		setLinearVelocity: function (e) {
			if (this.m_type == d.dynamics.B2Body.b2_staticBody) return;
			this.m_linearVelocity.setV(e)
		},
		getLocalCenter: function () {
			return this.m_sweep.localCenter
		},
		getWorldCenter: function () {
			return this.m_sweep.c
		},
		setAngle: function (e) {
			this.setPositionAndAngle(this.getPosition(), e)
		},
		getAngle: function () {
			return this.m_sweep.a
		},
		setPosition: function (e) {
			this.setPositionAndAngle(e, this.getAngle())
		},
		getPosition: function () {
			return this.m_xf.position
		},
		getTransform: function () {
			return this.m_xf
		},
		setTransform: function (e) {
			this.setPositionAndAngle(e.position, e.getAngle())
		},
		setPositionAndAngle: function (e, t) {
			var n;
			if (this.m_world.isLocked() == 1) return;
			this.m_xf.R.set(t),
			this.m_xf.position.setV(e);
			var r = this.m_xf.R,
			i = this.m_sweep.localCenter;
			this.m_sweep.c.x = r.col1.x * i.x + r.col2.x * i.y,
			this.m_sweep.c.y = r.col1.y * i.x + r.col2.y * i.y,
			this.m_sweep.c.x += this.m_xf.position.x,
			this.m_sweep.c.y += this.m_xf.position.y,
			this.m_sweep.c0.setV(this.m_sweep.c),
			this.m_sweep.a0 = this.m_sweep.a = t;
			var s = this.m_world.m_contactManager.m_broadPhase;
			n = this.m_fixtureList;
			while (n != null) n.synchronize(s, this.m_xf, this.m_xf),
			n = n.m_next;
			this.m_world.m_contactManager.findNewContacts()
		},
		DestroyFixture: function (e) {
			if (this.m_world.isLocked() == 1) return;
			var t = this.m_fixtureList,
			n = null,
			r = !1;
			while (t != null) {
				if (t == e) {
					n != null ? n.m_next = e.m_next: this.m_fixtureList = e.m_next,
					r = !0;
					break
				}
				n = t,
				t = t.m_next
			}
			var i = this.m_contactList;
			while (i != null) {
				var s = i.contact;
				i = i.next;
				var o = s.getFixtureA(),
				u = s.getFixtureB();
				(e == o || e == u) && this.m_world.m_contactManager.destroy(s)
			}
			if ((this.m_flags & d.dynamics.B2Body.e_activeFlag) != 0) {
				var a = this.m_world.m_contactManager.m_broadPhase;
				e.destroyProxy(a)
			}
			e.destroy(),
			e.m_body = null,
			e.m_next = null,
			--this.m_fixtureCount,
			this.resetMassData()
		},
		createFixture2: function (e, t) {
			t == null && (t = 0);
			var n = new d.dynamics.B2FixtureDef;
			return n.shape = e,
			n.density = t,
			this.createFixture(n)
		},
		createFixture: function (e) {
			if (this.m_world.isLocked() == 1) return null;
			var t = new d.dynamics.B2Fixture;
			t.create(this, this.m_xf, e);
			if ((this.m_flags & d.dynamics.B2Body.e_activeFlag) != 0) {
				var n = this.m_world.m_contactManager.m_broadPhase;
				t.createProxy(n, this.m_xf)
			}
			return t.m_next = this.m_fixtureList,
			this.m_fixtureList = t,
			++this.m_fixtureCount,
			t.m_body = this,
			t.m_density > 0 && this.resetMassData(),
			this.m_world.m_flags |= d.dynamics.B2World.e_newFixture,
			t
		},
		connectEdges: function (e, t, n) {
			var r = Math.atan2(t.getDirectionVector().y, t.getDirectionVector().x),
			i = Math.tan((r - n) * .5),
			s = d.common.math.B2Math.mulFV(i, t.getDirectionVector());
			s = d.common.math.B2Math.subtractVV(s, t.getNormalVector()),
			s = d.common.math.B2Math.mulFV(d.common.B2Settings.b2_toiSlop, s),
			s = d.common.math.B2Math.addVV(s, t.getVertex1());
			var o = d.common.math.B2Math.addVV(e.getDirectionVector(), t.getDirectionVector());
			o.normalize();
			var u = d.common.math.B2Math.dot(e.getDirectionVector(), t.getNormalVector()) > 0;
			return e.setNextEdge(t, s, o, u),
			t.setPrevEdge(e, s, o, u),
			r
		},
		__class__: d.dynamics.B2Body
	},
	d.dynamics.B2BodyDef = function () {
		this.position = new d.common.math.B2Vec2,
		this.linearVelocity = new d.common.math.B2Vec2,
		this.userData = null,
		this.angle = 0,
		this.angularVelocity = 0,
		this.linearDamping = 0,
		this.angularDamping = 0,
		this.allowSleep = !0,
		this.awake = !0,
		this.fixedRotation = !1,
		this.bullet = !1,
		this.type = d.dynamics.B2Body.b2_staticBody,
		this.active = !0,
		this.inertiaScale = 1
	},
	d.dynamics.B2BodyDef.__name__ = !0,
	d.dynamics.B2BodyDef.prototype = {
		__class__: d.dynamics.B2BodyDef
	},
	d.dynamics.B2ContactFilter = function () {},
	d.dynamics.B2ContactFilter.__name__ = !0,
	d.dynamics.B2ContactFilter.prototype = {
		rayCollide: function (e, t) {
			return e == null ? !0 : this.shouldCollide(y.Boot.__cast(e, d.dynamics.B2Fixture), t)
		},
		shouldCollide: function (e, t) {
			var n = e.getFilterData(),
			r = t.getFilterData();
			if (n.groupIndex == r.groupIndex && n.groupIndex != 0) return n.groupIndex > 0;
			var i = (n.maskBits & r.categoryBits) != 0 && (n.categoryBits & r.maskBits) != 0;
			return i
		},
		__class__: d.dynamics.B2ContactFilter
	},
	d.dynamics.B2ContactImpulse = function () {
		this.normalImpulses = new Array,
		this.tangentImpulses = new Array
	},
	d.dynamics.B2ContactImpulse.__name__ = !0,
	d.dynamics.B2ContactImpulse.prototype = {
		__class__: d.dynamics.B2ContactImpulse
	},
	d.dynamics.B2ContactListener = function () {},
	d.dynamics.B2ContactListener.__name__ = !0,
	d.dynamics.B2ContactListener.prototype = {
		postSolve: function (e, t) {},
		preSolve: function (e, t) {},
		endContact: function (e) {},
		beginContact: function (e) {},
		__class__: d.dynamics.B2ContactListener
	},
	d.dynamics.B2ContactManager = function () {
		this.m_world = null,
		this.m_contactCount = 0,
		this.m_contactFilter = d.dynamics.B2ContactFilter.b2_defaultFilter,
		this.m_contactListener = d.dynamics.B2ContactListener.b2_defaultListener,
		this.m_contactFactory = new d.dynamics.contacts.B2ContactFactory(this.m_allocator),
		this.m_broadPhase = new d.collision.B2DynamicTreeBroadPhase
	},
	d.dynamics.B2ContactManager.__name__ = !0,
	d.dynamics.B2ContactManager.prototype = {
		collide: function () {
			var e = this.m_world.m_contactList;
			while (e != null) {
				var t = e.getFixtureA(),
				n = e.getFixtureB(),
				r = t.getBody(),
				i = n.getBody();
				if (r.isAwake() == 0 && i.isAwake() == 0) {
					e = e.getNext();
					continue
				}
				if ((e.m_flags & d.dynamics.contacts.B2Contact.e_filterFlag) != 0) {
					if (i.shouldCollide(r) == 0) {
						var s = e;
						e = s.getNext(),
						this.destroy(s);
						continue
					}
					if (this.m_contactFilter.shouldCollide(t, n) == 0) {
						var s = e;
						e = s.getNext(),
						this.destroy(s);
						continue
					}
					e.m_flags &= ~d.dynamics.contacts.B2Contact.e_filterFlag
				}
				var o = t.m_proxy,
				u = n.m_proxy,
				a = this.m_broadPhase.testOverlap(o, u);
				if (a == 0) {
					var s = e;
					e = s.getNext(),
					this.destroy(s);
					continue
				}
				e.update(this.m_contactListener),
				e = e.getNext()
			}
		},
		destroy: function (e) {
			var t = e.getFixtureA(),
			n = e.getFixtureB(),
			r = t.getBody(),
			i = n.getBody();
			e.isTouching() && this.m_contactListener.endContact(e),
			e.m_prev != null && (e.m_prev.m_next = e.m_next),
			e.m_next != null && (e.m_next.m_prev = e.m_prev),
			e == this.m_world.m_contactList && (this.m_world.m_contactList = e.m_next),
			e.m_nodeA.prev != null && (e.m_nodeA.prev.next = e.m_nodeA.next),
			e.m_nodeA.next != null && (e.m_nodeA.next.prev = e.m_nodeA.prev),
			e.m_nodeA == r.m_contactList && (r.m_contactList = e.m_nodeA.next),
			e.m_nodeB.prev != null && (e.m_nodeB.prev.next = e.m_nodeB.next),
			e.m_nodeB.next != null && (e.m_nodeB.next.prev = e.m_nodeB.prev),
			e.m_nodeB == i.m_contactList && (i.m_contactList = e.m_nodeB.next),
			this.m_contactFactory.destroy(e),
			--this.m_contactCount
		},
		findNewContacts: function () {
			this.m_broadPhase.updatePairs(E(this, this.addPair))
		},
		addPair: function (e, t) {
			var n = y.Boot.__cast(e, d.dynamics.B2Fixture),
			r = y.Boot.__cast(t, d.dynamics.B2Fixture),
			i = n.getBody(),
			s = r.getBody();
			if (i == s) return;
			var o = s.getContactList();
			while (o != null) {
				if (o.other == i) {
					var u = o.contact.getFixtureA(),
					a = o.contact.getFixtureB();
					if (u == n && a == r) return;
					if (u == r && a == n) return
				}
				o = o.next
			}
			if (s.shouldCollide(i) == 0) return;
			if (this.m_contactFilter.shouldCollide(n, r) == 0) return;
			var f = this.m_contactFactory.create(n, r);
			n = f.getFixtureA(),
			r = f.getFixtureB(),
			i = n.m_body,
			s = r.m_body,
			f.m_prev = null,
			f.m_next = this.m_world.m_contactList,
			this.m_world.m_contactList != null && (this.m_world.m_contactList.m_prev = f),
			this.m_world.m_contactList = f,
			f.m_nodeA.contact = f,
			f.m_nodeA.other = s,
			f.m_nodeA.prev = null,
			f.m_nodeA.next = i.m_contactList,
			i.m_contactList != null && (i.m_contactList.prev = f.m_nodeA),
			i.m_contactList = f.m_nodeA,
			f.m_nodeB.contact = f,
			f.m_nodeB.other = i,
			f.m_nodeB.prev = null,
			f.m_nodeB.next = s.m_contactList,
			s.m_contactList != null && (s.m_contactList.prev = f.m_nodeB),
			s.m_contactList = f.m_nodeB,
			++this.m_world.m_contactCount;
			return
		},
		__class__: d.dynamics.B2ContactManager
	},
	d.dynamics.B2DebugDraw = function () {
		this.m_drawScale = 1,
		this.m_lineThickness = 1,
		this.m_alpha = 1,
		this.m_fillAlpha = 1,
		this.m_xformScale = 1,
		this.m_drawFlags = 0
	},
	d.dynamics.B2DebugDraw.__name__ = !0,
	d.dynamics.B2DebugDraw.prototype = {
		drawTransform: function (e) {},
		drawSegment: function (e, t, n) {
			this.m_ctx.beginPath(),
			this.m_ctx.moveTo(e.x * this.m_drawScale, e.y * this.m_drawScale),
			this.m_ctx.lineTo(t.x * this.m_drawScale, t.y * this.m_drawScale),
			this.m_ctx.closePath(),
			this.m_ctx.globalAlpha = this.m_alpha,
			this.m_ctx.strokeStyle = "#" + c.hex(n.get_color()),
			this.m_ctx.lineWidth = this.m_lineThickness,
			this.m_ctx.stroke()
		},
		drawSolidCircle: function (e, t, n, r) {
			this.m_ctx.beginPath(),
			this.m_ctx.arc(e.x * this.m_drawScale, e.y * this.m_drawScale, t * this.m_drawScale, 0, Math.PI * 2, !0),
			this.m_ctx.closePath(),
			this.m_ctx.globalAlpha = this.m_fillAlpha,
			this.m_ctx.fillStyle = "#" + c.hex(r.get_color()),
			this.m_ctx.fill(),
			this.m_ctx.globalAlpha = this.m_alpha,
			this.m_ctx.lineWidth = this.m_lineThickness,
			this.m_ctx.strokeStyle = "#" + c.hex(r.get_color()),
			this.m_ctx.stroke(),
			this.m_ctx.beginPath(),
			this.m_ctx.moveTo(e.x * this.m_drawScale, e.y * this.m_drawScale),
			this.m_ctx.lineTo((e.x + n.x * t) * this.m_drawScale, (e.y + n.y * t) * this.m_drawScale),
			this.m_ctx.closePath(),
			this.m_ctx.stroke()
		},
		drawCircle: function (e, t, n) {
			this.m_ctx.beginPath(),
			this.m_ctx.arc(e.x * this.m_drawScale, e.y * this.m_drawScale, t * this.m_drawScale, 0, Math.PI * 2, !0),
			this.m_ctx.closePath(),
			this.m_ctx.globalAlpha = this.m_alpha,
			this.m_ctx.lineWidth = this.m_lineThickness,
			this.m_ctx.strokeStyle = "#" + c.hex(n.get_color()),
			this.m_ctx.stroke()
		},
		drawSolidPolygon: function (e, t, n) {
			this.m_ctx.beginPath(),
			this.m_ctx.moveTo(e[0].x * this.m_drawScale, e[0].y * this.m_drawScale);
			var r = 1;
			while (r < t) {
				var i = r++;
				this.m_ctx.lineTo(e[i].x * this.m_drawScale, e[i].y * this.m_drawScale)
			}
			this.m_ctx.lineTo(e[0].x * this.m_drawScale, e[0].y * this.m_drawScale),
			this.m_ctx.closePath(),
			this.m_ctx.globalAlpha = this.m_alpha,
			this.m_ctx.strokeStyle = "#" + c.hex(n.get_color()),
			this.m_ctx.lineWidth = this.m_lineThickness,
			this.m_ctx.stroke(),
			this.m_ctx.globalAlpha = this.m_fillAlpha,
			this.m_ctx.fillStyle = "#" + c.hex(n.get_color()),
			this.m_ctx.fill()
		},
		drawPolygon: function (e, t, n) {
			this.m_ctx.beginPath(),
			this.m_ctx.moveTo(e[0].x * this.m_drawScale, e[0].y * this.m_drawScale);
			var r = 1;
			while (r < t) {
				var i = r++;
				this.m_ctx.lineTo(e[i].x * this.m_drawScale, e[i].y * this.m_drawScale)
			}
			this.m_ctx.lineTo(e[0].x * this.m_drawScale, e[0].y * this.m_drawScale),
			this.m_ctx.closePath(),
			this.m_ctx.globalAlpha = this.m_alpha,
			this.m_ctx.strokeStyle = "#" + c.hex(n.get_color()),
			this.m_ctx.lineWidth = this.m_lineThickness,
			this.m_ctx.stroke()
		},
		getXFormScale: function () {
			return this.m_xformScale
		},
		setXFormScale: function (e) {
			this.m_xformScale = e
		},
		getFillAlpha: function () {
			return this.m_fillAlpha
		},
		setFillAlpha: function (e) {
			this.m_fillAlpha = e
		},
		getAlpha: function () {
			return this.m_alpha
		},
		setAlpha: function (e) {
			this.m_alpha = e
		},
		getLineThickness: function () {
			return this.m_lineThickness
		},
		setLineThickness: function (e) {
			this.m_lineThickness = e
		},
		getDrawScale: function () {
			return this.m_drawScale
		},
		setDrawScale: function (e) {
			this.m_drawScale = e
		},
		setCanvas: function (e) {
			this.m_canvas = e,
			this.m_ctx = e.getContext("2d")
		},
		clearFlags: function (e) {
			this.m_drawFlags &= ~e
		},
		appendFlags: function (e) {
			this.m_drawFlags |= e
		},
		getFlags: function () {
			return this.m_drawFlags
		},
		setFlags: function (e) {
			this.m_drawFlags = e
		},
		__class__: d.dynamics.B2DebugDraw
	},
	d.dynamics.B2DestructionListener = function () {},
	d.dynamics.B2DestructionListener.__name__ = !0,
	d.dynamics.B2DestructionListener.prototype = {
		sayGoodbyeFixture: function (e) {},
		sayGoodbyeJoint: function (e) {},
		__class__: d.dynamics.B2DestructionListener
	},
	d.dynamics.B2FilterData = function () {
		this.categoryBits = 1,
		this.maskBits = 65535,
		this.groupIndex = 0
	},
	d.dynamics.B2FilterData.__name__ = !0,
	d.dynamics.B2FilterData.prototype = {
		copy: function () {
			var e = new d.dynamics.B2FilterData;
			return e.categoryBits = this.categoryBits,
			e.maskBits = this.maskBits,
			e.groupIndex = this.groupIndex,
			e
		},
		__class__: d.dynamics.B2FilterData
	},
	d.dynamics.B2Fixture = function () {
		this.m_filter = new d.dynamics.B2FilterData,
		this.m_aabb = new d.collision.B2AABB,
		this.m_userData = null,
		this.m_body = null,
		this.m_next = null,
		this.m_shape = null,
		this.m_density = 0,
		this.m_friction = 0,
		this.m_restitution = 0
	},
	d.dynamics.B2Fixture.__name__ = !0,
	d.dynamics.B2Fixture.prototype = {
		synchronize: function (e, t, n) {
			if (this.m_proxy == null) return;
			var r = new d.collision.B2AABB,
			i = new d.collision.B2AABB;
			this.m_shape.computeAABB(r, t),
			this.m_shape.computeAABB(i, n),
			this.m_aabb.combine(r, i);
			var s = d.common.math.B2Math.subtractVV(n.position, t.position);
			e.moveProxy(this.m_proxy, this.m_aabb, s)
		},
		destroyProxy: function (e) {
			if (this.m_proxy == null) return;
			e.destroyProxy(this.m_proxy),
			this.m_proxy = null
		},
		createProxy: function (e, t) {
			this.m_shape.computeAABB(this.m_aabb, t),
			this.m_proxy = e.createProxy(this.m_aabb, this)
		},
		destroy: function () {
			this.m_shape = null
		},
		create: function (e, t, n) {
			this.m_userData = n.userData,
			this.m_friction = n.friction,
			this.m_restitution = n.restitution,
			this.m_body = e,
			this.m_next = null,
			this.m_filter = n.filter.copy(),
			this.m_isSensor = n.isSensor,
			this.m_shape = n.shape.copy(),
			this.m_density = n.density
		},
		getAABB: function () {
			return this.m_aabb
		},
		setRestitution: function (e) {
			this.m_restitution = e
		},
		getRestitution: function () {
			return this.m_restitution
		},
		setFriction: function (e) {
			this.m_friction = e
		},
		getFriction: function () {
			return this.m_friction
		},
		getDensity: function () {
			return this.m_density
		},
		setDensity: function (e) {
			this.m_density = e
		},
		getMassData: function (e) {
			return e == null && (e = new d.collision.shapes.B2MassData),
			this.m_shape.computeMass(e, this.m_density),
			e
		},
		rayCast: function (e, t) {
			return this.m_shape.rayCast(e, t, this.m_body.getTransform())
		},
		testPoint: function (e) {
			return this.m_shape.testPoint(this.m_body.getTransform(), e)
		},
		SetUserData: function (e) {
			this.m_userData = e
		},
		getUserData: function () {
			return this.m_userData
		},
		getNext: function () {
			return this.m_next
		},
		getBody: function () {
			return this.m_body
		},
		getFilterData: function () {
			return this.m_filter.copy()
		},
		setFilterData: function (e) {
			this.m_filter = e.copy();
			if (this.m_body != null) return;
			var t = this.m_body.getContactList();
			while (t != null) {
				var n = t.contact,
				r = n.getFixtureA(),
				i = n.getFixtureB();
				(r == this || i == this) && n.flagForFiltering(),
				t = t.next
			}
		},
		isSensor: function () {
			return this.m_isSensor
		},
		setSensor: function (e) {
			if (this.m_isSensor == e) return;
			this.m_isSensor = e;
			if (this.m_body == null) return;
			var t = this.m_body.getContactList();
			while (t != null) {
				var n = t.contact,
				r = n.getFixtureA(),
				i = n.getFixtureB();
				(r == this || i == this) && n.setSensor(r.isSensor() || i.isSensor()),
				t = t.next
			}
		},
		getShape: function () {
			return this.m_shape
		},
		getType: function () {
			return this.m_shape.getType()
		},
		__class__: d.dynamics.B2Fixture
	},
	d.dynamics.B2FixtureDef = function () {
		this.filter = new d.dynamics.B2FilterData,
		this.shape = null,
		this.userData = null,
		this.friction = .2,
		this.restitution = 0,
		this.density = 0,
		this.filter.categoryBits = 1,
		this.filter.maskBits = 65535,
		this.filter.groupIndex = 0,
		this.isSensor = !1
	},
	d.dynamics.B2FixtureDef.__name__ = !0,
	d.dynamics.B2FixtureDef.prototype = {
		__class__: d.dynamics.B2FixtureDef
	},
	d.dynamics.B2Island = function () {
		this.m_bodies = new Array,
		this.m_contacts = new Array,
		this.m_joints = new Array
	},
	d.dynamics.B2Island.__name__ = !0,
	d.dynamics.B2Island.prototype = {
		addJoint: function (e) {
			this.m_joints[this.m_jointCount++] = e
		},
		addContact: function (e) {
			this.m_contacts[this.m_contactCount++] = e
		},
		addBody: function (e) {
			e.m_islandIndex = this.m_bodyCount,
			this.m_bodies[this.m_bodyCount++] = e
		},
		report: function (e) {
			if (this.m_listener == null) return;
			var t = 0,
			n = this.m_contactCount;
			while (t < n) {
				var r = t++,
				i = this.m_contacts[r],
				s = e[r],
				o = 0,
				u = s.pointCount;
				while (o < u) {
					var a = o++;
					d.dynamics.B2Island.s_impulse.normalImpulses[a] = s.points[a].normalImpulse,
					d.dynamics.B2Island.s_impulse.tangentImpulses[a] = s.points[a].tangentImpulse
				}
				this.m_listener.postSolve(i, d.dynamics.B2Island.s_impulse)
			}
		},
		solveTOI: function (e) {
			var t, n;
			this.m_contactSolver.initialize(e, this.m_contacts, this.m_contactCount, this.m_allocator);
			var r = this.m_contactSolver,
			i = 0,
			s = this.m_jointCount;
			while (i < s) {
				var o = i++;
				this.m_joints[o].initVelocityConstraints(e)
			}
			var i = 0,
			s = e.velocityIterations;
			while (i < s) {
				var o = i++;
				r.solveVelocityConstraints();
				var u = 0,
				a = this.m_jointCount;
				while (u < a) {
					var f = u++;
					this.m_joints[f].solveVelocityConstraints(e)
				}
			}
			var i = 0,
			s = this.m_bodyCount;
			while (i < s) {
				var o = i++,
				l = this.m_bodies[o];
				if (l.getType() == d.dynamics.B2Body.b2_staticBody) continue;
				var c = e.dt * l.m_linearVelocity.x,
				h = e.dt * l.m_linearVelocity.y;
				c * c + h * h > d.common.B2Settings.b2_maxTranslationSquared && (l.m_linearVelocity.normalize(), l.m_linearVelocity.x *= d.common.B2Settings.b2_maxTranslation * e.inv_dt, l.m_linearVelocity.y *= d.common.B2Settings.b2_maxTranslation * e.inv_dt);
				var p = e.dt * l.m_angularVelocity;
				p * p > d.common.B2Settings.b2_maxRotationSquared && (l.m_angularVelocity < 0 ? l.m_angularVelocity = -d.common.B2Settings.b2_maxRotation * e.inv_dt: l.m_angularVelocity = d.common.B2Settings.b2_maxRotation * e.inv_dt),
				l.m_sweep.c0.setV(l.m_sweep.c),
				l.m_sweep.a0 = l.m_sweep.a,
				l.m_sweep.c.x += e.dt * l.m_linearVelocity.x,
				l.m_sweep.c.y += e.dt * l.m_linearVelocity.y,
				l.m_sweep.a += e.dt * l.m_angularVelocity,
				l.synchronizeTransform()
			}
			var v = .75,
			i = 0,
			s = e.positionIterations;
			while (i < s) {
				var o = i++,
				m = r.solvePositionConstraints(v),
				g = !0,
				u = 0,
				a = this.m_jointCount;
				while (u < a) {
					var f = u++,
					y = this.m_joints[f].solvePositionConstraints(d.common.B2Settings.b2_contactBaumgarte);
					g = g && y
				}
				if (m && g) break
			}
			this.report(r.m_constraints)
		},
		solve: function (e, t, n) {
			var r, i, s, o, u = 0,
			a = this.m_bodyCount;
			while (u < a) {
				var f = u++;
				s = this.m_bodies[f];
				if (s.getType() != d.dynamics.B2Body.b2_dynamicBody) continue;
				s.m_linearVelocity.x += e.dt * (t.x + s.m_invMass * s.m_force.x),
				s.m_linearVelocity.y += e.dt * (t.y + s.m_invMass * s.m_force.y),
				s.m_angularVelocity += e.dt * s.m_invI * s.m_torque,
				s.m_linearVelocity.multiply(d.common.math.B2Math.clamp(1 - e.dt * s.m_linearDamping, 0, 1)),
				s.m_angularVelocity *= d.common.math.B2Math.clamp(1 - e.dt * s.m_angularDamping, 0, 1)
			}
			this.m_contactSolver.initialize(e, this.m_contacts, this.m_contactCount, this.m_allocator);
			var l = this.m_contactSolver;
			l.initVelocityConstraints(e);
			var u = 0,
			a = this.m_jointCount;
			while (u < a) {
				var f = u++;
				o = this.m_joints[f],
				o.initVelocityConstraints(e)
			}
			var u = 0,
			a = e.velocityIterations;
			while (u < a) {
				var f = u++,
				c = 0,
				h = this.m_jointCount;
				while (c < h) {
					var p = c++;
					o = this.m_joints[p],
					o.solveVelocityConstraints(e)
				}
				l.solveVelocityConstraints()
			}
			var u = 0,
			a = this.m_jointCount;
			while (u < a) {
				var f = u++;
				o = this.m_joints[f],
				o.finalizeVelocityConstraints()
			}
			l.finalizeVelocityConstraints();
			var u = 0,
			a = this.m_bodyCount;
			while (u < a) {
				var f = u++;
				s = this.m_bodies[f];
				if (s.getType() == d.dynamics.B2Body.b2_staticBody) continue;
				var v = e.dt * s.m_linearVelocity.x,
				m = e.dt * s.m_linearVelocity.y;
				v * v + m * m > d.common.B2Settings.b2_maxTranslationSquared && (s.m_linearVelocity.normalize(), s.m_linearVelocity.x *= d.common.B2Settings.b2_maxTranslation * e.inv_dt, s.m_linearVelocity.y *= d.common.B2Settings.b2_maxTranslation * e.inv_dt);
				var g = e.dt * s.m_angularVelocity;
				g * g > d.common.B2Settings.b2_maxRotationSquared && (s.m_angularVelocity < 0 ? s.m_angularVelocity = -d.common.B2Settings.b2_maxRotation * e.inv_dt: s.m_angularVelocity = d.common.B2Settings.b2_maxRotation * e.inv_dt),
				s.m_sweep.c0.setV(s.m_sweep.c),
				s.m_sweep.a0 = s.m_sweep.a,
				s.m_sweep.c.x += e.dt * s.m_linearVelocity.x,
				s.m_sweep.c.y += e.dt * s.m_linearVelocity.y,
				s.m_sweep.a += e.dt * s.m_angularVelocity,
				s.synchronizeTransform()
			}
			var u = 0,
			a = e.positionIterations;
			while (u < a) {
				var f = u++,
				y = l.solvePositionConstraints(d.common.B2Settings.b2_contactBaumgarte),
				b = !0,
				c = 0,
				h = this.m_jointCount;
				while (c < h) {
					var p = c++;
					o = this.m_joints[p];
					var w = o.solvePositionConstraints(d.common.B2Settings.b2_contactBaumgarte);
					b = b && w
				}
				if (y && b) break
			}
			this.report(l.m_constraints);
			if (n) {
				var E = 1.7976931348623157e308,
				S = d.common.B2Settings.b2_linearSleepTolerance * d.common.B2Settings.b2_linearSleepTolerance,
				x = d.common.B2Settings.b2_angularSleepTolerance * d.common.B2Settings.b2_angularSleepTolerance,
				u = 0,
				a = this.m_bodyCount;
				while (u < a) {
					var f = u++;
					s = this.m_bodies[f];
					if (s.getType() == d.dynamics.B2Body.b2_staticBody) continue;
					(s.m_flags & d.dynamics.B2Body.e_allowSleepFlag) == 0 && (s.m_sleepTime = 0, E = 0),
					(s.m_flags & d.dynamics.B2Body.e_allowSleepFlag) == 0 || s.m_angularVelocity * s.m_angularVelocity > x || d.common.math.B2Math.dot(s.m_linearVelocity, s.m_linearVelocity) > S ? (s.m_sleepTime = 0, E = 0) : (s.m_sleepTime += e.dt, E = d.common.math.B2Math.min(E, s.m_sleepTime))
				}
				if (E >= d.common.B2Settings.b2_timeToSleep) {
					var u = 0,
					a = this.m_bodyCount;
					while (u < a) {
						var f = u++;
						s = this.m_bodies[f],
						s.setAwake(!1)
					}
				}
			}
		},
		clear: function () {
			this.m_bodyCount = 0,
			this.m_contactCount = 0,
			this.m_jointCount = 0
		},
		initialize: function (e, t, n, r, i, s) {
			var o;
			this.m_bodyCapacity = e,
			this.m_contactCapacity = t,
			this.m_jointCapacity = n,
			this.m_bodyCount = 0,
			this.m_contactCount = 0,
			this.m_jointCount = 0,
			this.m_allocator = r,
			this.m_listener = i,
			this.m_contactSolver = s;
			var u = this.m_bodies.length;
			while (u < e) {
				var a = u++;
				this.m_bodies[a] = null
			}
			var u = this.m_contacts.length;
			while (u < t) {
				var a = u++;
				this.m_contacts[a] = null
			}
			var u = this.m_joints.length;
			while (u < n) {
				var a = u++;
				this.m_joints[a] = null
			}
		},
		__class__: d.dynamics.B2Island
	},
	d.dynamics.B2TimeStep = function () {},
	d.dynamics.B2TimeStep.__name__ = !0,
	d.dynamics.B2TimeStep.prototype = {
		set: function (e) {
			this.dt = e.dt,
			this.inv_dt = e.inv_dt,
			this.positionIterations = e.positionIterations,
			this.velocityIterations = e.velocityIterations,
			this.warmStarting = e.warmStarting
		},
		__class__: d.dynamics.B2TimeStep
	},
	d.dynamics.B2World = function (e, t) {
		this.s_stack = new Array,
		this.m_contactManager = new d.dynamics.B2ContactManager,
		this.m_contactSolver = new d.dynamics.contacts.B2ContactSolver,
		this.m_island = new d.dynamics.B2Island,
		this.m_destructionListener = null,
		this.m_debugDraw = null,
		this.m_bodyList = null,
		this.m_contactList = null,
		this.m_jointList = null,
		this.m_controllerList = null,
		this.m_bodyCount = 0,
		this.m_contactCount = 0,
		this.m_jointCount = 0,
		this.m_controllerCount = 0,
		d.dynamics.B2World.m_warmStarting = !0,
		d.dynamics.B2World.m_continuousPhysics = !0,
		this.m_allowSleep = t,
		this.m_gravity = e,
		this.m_inv_dt0 = 0,
		this.m_contactManager.m_world = this;
		var n = new d.dynamics.B2BodyDef;
		this.m_groundBody = this.createBody(n)
	},
	d.dynamics.B2World.__name__ = !0,
	d.dynamics.B2World.prototype = {
		drawShape: function (e, t, n) {
			switch (e.m_type) {
			case d.collision.shapes.B2Shape.e_circleShape:
				var r = y.Boot.__cast(e, d.collision.shapes.B2CircleShape),
				i = d.common.math.B2Math.mulX(t, r.m_p),
				s = r.m_radius,
				o = t.R.col1;
				this.m_debugDraw.drawSolidCircle(i, s, o, n);
				break;
			case d.collision.shapes.B2Shape.e_polygonShape:
				var u, a = y.Boot.__cast(e, d.collision.shapes.B2PolygonShape),
				f = a.getVertexCount(),
				l = a.getVertices(),
				c = new Array,
				h = 0;
				while (h < f) {
					var p = h++;
					c[p] = d.common.math.B2Math.mulX(t, l[p])
				}
				this.m_debugDraw.drawSolidPolygon(c, f, n);
				break;
			case d.collision.shapes.B2Shape.e_edgeShape:
				var v = y.Boot.__cast(e, d.collision.shapes.B2EdgeShape);
				this.m_debugDraw.drawSegment(d.common.math.B2Math.mulX(t, v.getVertex1()), d.common.math.B2Math.mulX(t, v.getVertex2()), n)
			}
		},
		drawJoint: function (e) {
			var t = e.getBodyA(),
			n = e.getBodyB(),
			r = t.m_xf,
			i = n.m_xf,
			s = r.position,
			o = i.position,
			u = e.getAnchorA(),
			a = e.getAnchorB(),
			f = d.dynamics.B2World.s_jointColor;
			switch (e.m_type) {
			case d.dynamics.joints.B2Joint.e_distanceJoint:
				this.m_debugDraw.drawSegment(u, a, f);
				break;
			case d.dynamics.joints.B2Joint.e_pulleyJoint:
				var l = y.Boot.__cast(e, d.dynamics.joints.B2PulleyJoint),
				c = l.getGroundAnchorA(),
				h = l.getGroundAnchorB();
				this.m_debugDraw.drawSegment(c, u, f),
				this.m_debugDraw.drawSegment(h, a, f),
				this.m_debugDraw.drawSegment(c, h, f);
				break;
			case d.dynamics.joints.B2Joint.e_mouseJoint:
				this.m_debugDraw.drawSegment(u, a, f);
				break;
			default:
				t != this.m_groundBody && this.m_debugDraw.drawSegment(s, u, f),
				this.m_debugDraw.drawSegment(u, a, f),
				n != this.m_groundBody && this.m_debugDraw.drawSegment(o, a, f)
			}
		},
		solveTOI: function (e) {
			var t, n, r, i, s, o, u, a = this.m_island;
			a.initialize(this.m_bodyCount, d.common.B2Settings.b2_maxTOIContactsPerIsland, d.common.B2Settings.b2_maxTOIJointsPerIsland, null, this.m_contactManager.m_contactListener, this.m_contactSolver);
			var f = d.dynamics.B2World.s_queue;
			t = this.m_bodyList;
			while (t != null) t.m_flags &= ~d.dynamics.B2Body.e_islandFlag,
			t.m_sweep.t0 = 0,
			t = t.m_next;
			var l = this.m_contactList;
			while (l != null) l.m_flags &= ~ (d.dynamics.contacts.B2Contact.e_toiFlag | d.dynamics.contacts.B2Contact.e_islandFlag),
			l = l.m_next;
			u = this.m_jointList;
			while (u != null) u.m_islandFlag = !1,
			u = u.m_next;
			for (;;) {
				var c = null,
				h = 1;
				l = this.m_contactList;
				while (l != null) {
					if (l.isSensor() == 1 || l.isEnabled() == 0 || l.isContinuous() == 0) {
						l = l.m_next;
						continue
					}
					var p = 1;
					if ((l.m_flags & d.dynamics.contacts.B2Contact.e_toiFlag) != 0) p = l.m_toi;
					else {
						n = l.m_fixtureA,
						r = l.m_fixtureB,
						i = n.m_body,
						s = r.m_body;
						if (! (i.getType() == d.dynamics.B2Body.b2_dynamicBody && i.isAwake() != 0 || s.getType() == d.dynamics.B2Body.b2_dynamicBody && s.isAwake() != 0)) {
							l = l.m_next;
							continue
						}
						var v = i.m_sweep.t0;
						i.m_sweep.t0 < s.m_sweep.t0 ? (v = s.m_sweep.t0, i.m_sweep.advance(v)) : s.m_sweep.t0 < i.m_sweep.t0 && (v = i.m_sweep.t0, s.m_sweep.advance(v)),
						p = l.computeTOI(i.m_sweep, s.m_sweep),
						d.common.B2Settings.b2Assert(0 <= p && p <= 1),
						p > 0 && p < 1 && (p = (1 - p) * v + p, p > 1 && (p = 1)),
						l.m_toi = p,
						l.m_flags |= d.dynamics.contacts.B2Contact.e_toiFlag
					}
					2.2250738585072014e-308 < p && p < h && (c = l, h = p),
					l = l.m_next
				}
				if (c == null || 1 < h) break;
				n = c.m_fixtureA,
				r = c.m_fixtureB,
				i = n.m_body,
				s = r.m_body,
				d.dynamics.B2World.s_backupA.set(i.m_sweep),
				d.dynamics.B2World.s_backupB.set(s.m_sweep),
				i.advance(h),
				s.advance(h),
				c.update(this.m_contactManager.m_contactListener),
				c.m_flags &= ~d.dynamics.contacts.B2Contact.e_toiFlag;
				if (c.isSensor() == 1 || c.isEnabled() == 0) {
					i.m_sweep.set(d.dynamics.B2World.s_backupA),
					s.m_sweep.set(d.dynamics.B2World.s_backupB),
					i.synchronizeTransform(),
					s.synchronizeTransform();
					continue
				}
				if (c.isTouching() == 0) continue;
				var m = i;
				m.getType() != d.dynamics.B2Body.b2_dynamicBody && (m = s),
				a.clear();
				var g = 0,
				y = 0;
				f[g + y++] = m,
				m.m_flags |= d.dynamics.B2Body.e_islandFlag;
				while (y > 0) {
					t = f[g++],
					--y,
					a.addBody(t),
					t.isAwake() == 0 && t.setAwake(!0);
					if (t.getType() != d.dynamics.B2Body.b2_dynamicBody) continue;
					o = t.m_contactList;
					var b;
					while (o != null) {
						if (a.m_contactCount == a.m_contactCapacity) {
							o = o.next;
							break
						}
						if ((o.contact.m_flags & d.dynamics.contacts.B2Contact.e_islandFlag) != 0) {
							o = o.next;
							continue
						}
						if (o.contact.isSensor() == 1 || o.contact.isEnabled() == 0 || o.contact.isTouching() == 0) {
							o = o.next;
							continue
						}
						a.addContact(o.contact),
						o.contact.m_flags |= d.dynamics.contacts.B2Contact.e_islandFlag,
						b = o.other;
						if ((b.m_flags & d.dynamics.B2Body.e_islandFlag) != 0) {
							o = o.next;
							continue
						}
						b.getType() != d.dynamics.B2Body.b2_staticBody && (b.advance(h), b.setAwake(!0)),
						f[g + y] = b,
						++y,
						b.m_flags |= d.dynamics.B2Body.e_islandFlag,
						o = o.next
					}
					var w = t.m_jointList;
					while (w != null) {
						if (a.m_jointCount == a.m_jointCapacity) {
							w = w.next;
							continue
						}
						if (w.joint.m_islandFlag == 1) {
							w = w.next;
							continue
						}
						b = w.other;
						if (b.isActive() == 0) {
							w = w.next;
							continue
						}
						a.addJoint(w.joint),
						w.joint.m_islandFlag = !0;
						if ((b.m_flags & d.dynamics.B2Body.e_islandFlag) != 0) {
							w = w.next;
							continue
						}
						b.getType() != d.dynamics.B2Body.b2_staticBody && (b.advance(h), b.setAwake(!0)),
						f[g + y] = b,
						++y,
						b.m_flags |= d.dynamics.B2Body.e_islandFlag,
						w = w.next
					}
				}
				var E = d.dynamics.B2World.s_timestep;
				E.warmStarting = !1,
				E.dt = (1 - h) * e.dt,
				E.inv_dt = 1 / E.dt,
				E.dtRatio = 0,
				E.velocityIterations = e.velocityIterations,
				E.positionIterations = e.positionIterations,
				a.solveTOI(E);
				var S, x = 0,
				T = a.m_bodyCount;
				while (x < T) {
					var N = x++;
					t = a.m_bodies[N],
					t.m_flags &= ~d.dynamics.B2Body.e_islandFlag;
					if (t.isAwake() == 0) continue;
					if (t.getType() != d.dynamics.B2Body.b2_dynamicBody) continue;
					t.synchronizeFixtures(),
					o = t.m_contactList;
					while (o != null) o.contact.m_flags &= ~d.dynamics.contacts.B2Contact.e_toiFlag,
					o = o.next
				}
				var x = 0,
				T = a.m_contactCount;
				while (x < T) {
					var N = x++;
					l = a.m_contacts[N],
					l.m_flags &= ~ (d.dynamics.contacts.B2Contact.e_toiFlag | d.dynamics.contacts.B2Contact.e_islandFlag)
				}
				var x = 0,
				T = a.m_jointCount;
				while (x < T) {
					var N = x++;
					u = a.m_joints[N],
					u.m_islandFlag = !1
				}
				this.m_contactManager.findNewContacts()
			}
		},
		solve: function (e) {
			var t, n = this.m_controllerList;
			while (n != null) n.step(e),
			n = n.m_next;
			var r = this.m_island;
			r.initialize(this.m_bodyCount, this.m_contactCount, this.m_jointCount, null, this.m_contactManager.m_contactListener, this.m_contactSolver),
			t = this.m_bodyList;
			while (t != null) t.m_flags &= ~d.dynamics.B2Body.e_islandFlag,
			t = t.m_next;
			var i = this.m_contactList;
			while (i != null) i.m_flags &= ~d.dynamics.contacts.B2Contact.e_islandFlag,
			i = i.m_next;
			var s = this.m_jointList;
			while (s != null) s.m_islandFlag = !1,
			s = s.m_next;
			var o = this.m_bodyCount,
			u = this.s_stack,
			a = this.m_bodyList;
			while (a != null) {
				if ((a.m_flags & d.dynamics.B2Body.e_islandFlag) != 0) {
					a = a.m_next;
					continue
				}
				if (a.isAwake() == 0 || a.isActive() == 0) {
					a = a.m_next;
					continue
				}
				if (a.getType() == d.dynamics.B2Body.b2_staticBody) {
					a = a.m_next;
					continue
				}
				r.clear();
				var f = 0;
				u[f++] = a,
				a.m_flags |= d.dynamics.B2Body.e_islandFlag;
				while (f > 0) {
					t = u[--f],
					r.addBody(t),
					t.isAwake() == 0 && t.setAwake(!0);
					if (t.getType() == d.dynamics.B2Body.b2_staticBody) continue;
					var l, c = t.m_contactList;
					while (c != null) {
						if ((c.contact.m_flags & d.dynamics.contacts.B2Contact.e_islandFlag) != 0) {
							c = c.next;
							continue
						}
						if (c.contact.isSensor() == 1 || c.contact.isEnabled() == 0 || c.contact.isTouching() == 0) {
							c = c.next;
							continue
						}
						r.addContact(c.contact),
						c.contact.m_flags |= d.dynamics.contacts.B2Contact.e_islandFlag,
						l = c.other;
						if ((l.m_flags & d.dynamics.B2Body.e_islandFlag) != 0) {
							c = c.next;
							continue
						}
						u[f++] = l,
						l.m_flags |= d.dynamics.B2Body.e_islandFlag,
						c = c.next
					}
					var h = t.m_jointList;
					while (h != null) {
						if (h.joint.m_islandFlag == 1) {
							h = h.next;
							continue
						}
						l = h.other;
						if (l.isActive() == 0) {
							h = h.next;
							continue
						}
						r.addJoint(h.joint),
						h.joint.m_islandFlag = !0;
						if ((l.m_flags & d.dynamics.B2Body.e_islandFlag) != 0) {
							h = h.next;
							continue
						}
						u[f++] = l,
						l.m_flags |= d.dynamics.B2Body.e_islandFlag,
						h = h.next
					}
				}
				r.solve(e, this.m_gravity, this.m_allowSleep);
				var p = 0,
				v = r.m_bodyCount;
				while (p < v) {
					var m = p++;
					t = r.m_bodies[m],
					t.getType() == d.dynamics.B2Body.b2_staticBody && (t.m_flags &= ~d.dynamics.B2Body.e_islandFlag)
				}
				a = a.m_next
			}
			var p = 0,
			v = u.length;
			while (p < v) {
				var m = p++;
				if (u[m] == null) break;
				u[m] = null
			}
			t = this.m_bodyList;
			while (t != null) {
				if (t.isAwake() == 0 || t.isActive() == 0) {
					t = t.m_next;
					continue
				}
				if (t.getType() == d.dynamics.B2Body.b2_staticBody) {
					t = t.m_next;
					continue
				}
				t.synchronizeFixtures(),
				t = t.m_next
			}
			this.m_contactManager.findNewContacts()
		},
		isLocked: function () {
			return (this.m_flags & d.dynamics.B2World.e_locked) > 0
		},
		getContactList: function () {
			return this.m_contactList
		},
		getJointList: function () {
			return this.m_jointList
		},
		getBodyList: function () {
			return this.m_bodyList
		},
		rayCastAll: function (e, t) {
			var n = new Array,
			r = function (e, t, r, i) {
				return n[n.length] = e,
				1
			};
			return this.rayCast(r, e, t),
			n
		},
		rayCastOne: function (e, t) {
			var n, r = function (e, t, r, i) {
				return n = e,
				i
			};
			return this.rayCast(r, e, t),
			n
		},
		rayCast: function (e, t, n) {
			var r = this.m_contactManager.m_broadPhase,
			i = new d.collision.B2RayCastOutput,
			s = function (s, o) {
				var u = r.getUserData(o),
				a = y.Boot.__cast(u, d.dynamics.B2Fixture),
				f = a.rayCast(i, s);
				if (f) {
					var l = i.fraction,
					c = new d.common.math.B2Vec2((1 - l) * t.x + l * n.x, (1 - l) * t.y + l * n.y);
					return e(a, c, i.normal, l)
				}
				return s.maxFraction
			},
			o = new d.collision.B2RayCastInput(t, n);
			r.rayCast(s, o)
		},
		queryPoint: function (e, t) {
			var n = this.m_contactManager.m_broadPhase,
			r = function (r) {
				var i = y.Boot.__cast(n.getUserData(r), d.dynamics.B2Fixture);
				return i.testPoint(t) ? e(i) : !0
			},
			i = new d.collision.B2AABB;
			i.lowerBound.set(t.x - d.common.B2Settings.b2_linearSlop, t.y - d.common.B2Settings.b2_linearSlop),
			i.upperBound.set(t.x + d.common.B2Settings.b2_linearSlop, t.y + d.common.B2Settings.b2_linearSlop),
			n.query(r, i)
		},
		queryShape: function (e, t, n) {
			n == null && (n = new d.common.math.B2Transform, n.setIdentity());
			var r = this.m_contactManager.m_broadPhase,
			i = function (i) {
				var s = y.Boot.__cast(r.getUserData(i), d.dynamics.B2Fixture);
				return d.collision.shapes.B2Shape.testOverlap(t, n, s.getShape(), s.getBody().getTransform()) ? e(s) : !0
			},
			s = new d.collision.B2AABB;
			t.computeAABB(s, n),
			r.query(i, s)
		},
		queryAABB: function (e, t) {
			var n = this.m_contactManager.m_broadPhase,
			r = function (t) {
				return e(n.getUserData(t))
			};
			n.query(r, t)
		},
		drawDebugData: function () {
			if (this.m_debugDraw == null) return;
			this.m_debugDraw.m_ctx.clearRect(0, 0, 1e3, 1e3);
			var e = this.m_debugDraw.getFlags(),
			t,
			n,
			r,
			i,
			s,
			o,
			u = new d.common.math.B2Vec2,
			a = new d.common.math.B2Vec2,
			f = new d.common.math.B2Vec2,
			l,
			c = new d.collision.B2AABB,
			h = new d.collision.B2AABB,
			p = [new d.common.math.B2Vec2, new d.common.math.B2Vec2, new d.common.math.B2Vec2, new d.common.math.B2Vec2],
			v = new d.common.B2Color(0, 0, 0);
			if ((e & d.dynamics.B2DebugDraw.e_shapeBit) != 0) {
				n = this.m_bodyList;
				while (n != null) {
					l = n.m_xf,
					r = n.getFixtureList();
					while (r != null) i = r.getShape(),
					n.isActive() == 0 ? (v.set(.5, .5, .3), this.drawShape(i, l, v)) : n.getType() == d.dynamics.B2Body.b2_staticBody ? (v.set(.5, .9, .5), this.drawShape(i, l, v)) : n.getType() == d.dynamics.B2Body.b2_kinematicBody ? (v.set(.5, .5, .9), this.drawShape(i, l, v)) : n.isAwake() == 0 ? (v.set(.6, .6, .6), this.drawShape(i, l, v)) : (v.set(.9, .7, .7), this.drawShape(i, l, v)),
					r = r.m_next;
					n = n.m_next
				}
			}
			if ((e & d.dynamics.B2DebugDraw.e_jointBit) != 0) {
				s = this.m_jointList;
				while (s != null) this.drawJoint(s),
				s = s.m_next
			}
			if ((e & d.dynamics.B2DebugDraw.e_controllerBit) != 0) {
				var m = this.m_controllerList;
				while (m != null) m.draw(this.m_debugDraw),
				m = m.m_next
			}
			if ((e & d.dynamics.B2DebugDraw.e_pairBit) != 0) {
				v.set(.3, .9, .9);
				var g = this.m_contactManager.m_contactList;
				while (g != null) {
					var y = g.getFixtureA(),
					b = g.getFixtureB(),
					w = y.getAABB().getCenter(),
					E = b.getAABB().getCenter();
					this.m_debugDraw.drawSegment(w, E, v),
					g = g.getNext()
				}
			}
			if ((e & d.dynamics.B2DebugDraw.e_aabbBit) != 0) {
				o = this.m_contactManager.m_broadPhase,
				p = [new d.common.math.B2Vec2, new d.common.math.B2Vec2, new d.common.math.B2Vec2, new d.common.math.B2Vec2],
				n = this.m_bodyList;
				while (n != null) {
					if (n.isActive() == 0) {
						n = n.getNext();
						continue
					}
					r = n.getFixtureList();
					while (r != null) {
						var S = o.getFatAABB(r.m_proxy);
						p[0].set(S.lowerBound.x, S.lowerBound.y),
						p[1].set(S.upperBound.x, S.lowerBound.y),
						p[2].set(S.upperBound.x, S.upperBound.y),
						p[3].set(S.lowerBound.x, S.upperBound.y),
						this.m_debugDraw.drawPolygon(p, 4, v),
						r = r.getNext()
					}
					n = n.getNext()
				}
			}
			if ((e & d.dynamics.B2DebugDraw.e_centerOfMassBit) != 0) {
				n = this.m_bodyList;
				while (n != null) l = d.dynamics.B2World.s_xf,
				l.R = n.m_xf.R,
				l.position = n.getWorldCenter(),
				this.m_debugDraw.drawTransform(l),
				n = n.m_next
			}
		},
		clearForces: function () {
			var e = this.m_bodyList;
			while (e != null) e.m_force.setZero(),
			e.m_torque = 0,
			e = e.m_next
		},
		step: function (e, t, n) { (this.m_flags & d.dynamics.B2World.e_newFixture) != 0 && (this.m_contactManager.findNewContacts(), this.m_flags &= ~d.dynamics.B2World.e_newFixture),
			this.m_flags |= d.dynamics.B2World.e_locked;
			var r = d.dynamics.B2World.s_timestep2;
			r.dt = e,
			r.velocityIterations = t,
			r.positionIterations = n,
			e > 0 ? r.inv_dt = 1 / e: r.inv_dt = 0,
			r.dtRatio = this.m_inv_dt0 * e,
			r.warmStarting = d.dynamics.B2World.m_warmStarting,
			this.m_contactManager.collide(),
			r.dt > 0 && this.solve(r),
			d.dynamics.B2World.m_continuousPhysics && r.dt > 0 && this.solveTOI(r),
			r.dt > 0 && (this.m_inv_dt0 = r.inv_dt),
			this.m_flags &= ~d.dynamics.B2World.e_locked
		},
		getGroundBody: function () {
			return this.m_groundBody
		},
		getGravity: function () {
			return this.m_gravity
		},
		setGravity: function (e) {
			this.m_gravity = e
		},
		getContactCount: function () {
			return this.m_contactCount
		},
		getJointCount: function () {
			return this.m_jointCount
		},
		getBodyCount: function () {
			return this.m_bodyCount
		},
		setContinuousPhysics: function (e) {
			d.dynamics.B2World.m_continuousPhysics = e
		},
		setWarmStarting: function (e) {
			d.dynamics.B2World.m_warmStarting = e
		},
		destroyController: function (e) {
			e.clear(),
			e.m_next != null && (e.m_next.m_prev = e.m_prev),
			e.m_prev != null && (e.m_prev.m_next = e.m_next),
			e == this.m_controllerList && (this.m_controllerList = e.m_next),
			--this.m_controllerCount
		},
		createController: function (e) {
			if (e.m_world != this) throw "Controller can only be a member of one world";
			return e.m_next = this.m_controllerList,
			e.m_prev = null,
			this.m_controllerList != null && (this.m_controllerList.m_prev = e),
			this.m_controllerList = e,
			++this.m_controllerCount,
			e.m_world = this,
			e
		},
		removeController: function (e) {
			e.m_prev != null && (e.m_prev.m_next = e.m_next),
			e.m_next != null && (e.m_next.m_prev = e.m_prev),
			this.m_controllerList == e && (this.m_controllerList = e.m_next),
			this.m_controllerCount--
		},
		addController: function (e) {
			return e.m_next = this.m_controllerList,
			e.m_prev = null,
			this.m_controllerList = e,
			e.m_world = this,
			this.m_controllerCount++,
			e
		},
		destroyJoint: function (e) {
			var t = e.m_collideConnected;
			e.m_prev != null && (e.m_prev.m_next = e.m_next),
			e.m_next != null && (e.m_next.m_prev = e.m_prev),
			e == this.m_jointList && (this.m_jointList = e.m_next);
			var n = e.m_bodyA,
			r = e.m_bodyB;
			n.setAwake(!0),
			r.setAwake(!0),
			e.m_edgeA.prev != null && (e.m_edgeA.prev.next = e.m_edgeA.next),
			e.m_edgeA.next != null && (e.m_edgeA.next.prev = e.m_edgeA.prev),
			e.m_edgeA == n.m_jointList && (n.m_jointList = e.m_edgeA.next),
			e.m_edgeA.prev = null,
			e.m_edgeA.next = null,
			e.m_edgeB.prev != null && (e.m_edgeB.prev.next = e.m_edgeB.next),
			e.m_edgeB.next != null && (e.m_edgeB.next.prev = e.m_edgeB.prev),
			e.m_edgeB == r.m_jointList && (r.m_jointList = e.m_edgeB.next),
			e.m_edgeB.prev = null,
			e.m_edgeB.next = null,
			d.dynamics.joints.B2Joint.destroy(e, null),
			--this.m_jointCount;
			if (t == 0) {
				var i = r.getContactList();
				while (i != null) i.other == n && i.contact.flagForFiltering(),
				i = i.next
			}
		},
		createJoint: function (e) {
			var t = d.dynamics.joints.B2Joint.create(e, null);
			t.m_prev = null,
			t.m_next = this.m_jointList,
			this.m_jointList != null && (this.m_jointList.m_prev = t),
			this.m_jointList = t,
			++this.m_jointCount,
			t.m_edgeA.joint = t,
			t.m_edgeA.other = t.m_bodyB,
			t.m_edgeA.prev = null,
			t.m_edgeA.next = t.m_bodyA.m_jointList,
			t.m_bodyA.m_jointList != null && (t.m_bodyA.m_jointList.prev = t.m_edgeA),
			t.m_bodyA.m_jointList = t.m_edgeA,
			t.m_edgeB.joint = t,
			t.m_edgeB.other = t.m_bodyA,
			t.m_edgeB.prev = null,
			t.m_edgeB.next = t.m_bodyB.m_jointList,
			t.m_bodyB.m_jointList != null && (t.m_bodyB.m_jointList.prev = t.m_edgeB),
			t.m_bodyB.m_jointList = t.m_edgeB;
			var n = e.bodyA,
			r = e.bodyB;
			if (e.collideConnected == 0) {
				var i = r.getContactList();
				while (i != null) i.other == n && i.contact.flagForFiltering(),
				i = i.next
			}
			return t
		},
		destroyBody: function (e) {
			if (this.isLocked() == 1) return;
			var t = e.m_jointList;
			while (t != null) {
				var n = t;
				t = t.next,
				this.m_destructionListener != null && this.m_destructionListener.sayGoodbyeJoint(n.joint),
				this.destroyJoint(n.joint)
			}
			var r = e.m_controllerList;
			while (r != null) {
				var i = r;
				r = r.nextController,
				i.controller.removeBody(e)
			}
			var s = e.m_contactList;
			while (s != null) {
				var o = s;
				s = s.next,
				this.m_contactManager.destroy(o.contact)
			}
			e.m_contactList = null;
			var u = e.m_fixtureList;
			while (u != null) {
				var a = u;
				u = u.m_next,
				this.m_destructionListener != null && this.m_destructionListener.sayGoodbyeFixture(a),
				a.destroyProxy(this.m_contactManager.m_broadPhase),
				a.destroy()
			}
			e.m_fixtureList = null,
			e.m_fixtureCount = 0,
			e.m_prev != null && (e.m_prev.m_next = e.m_next),
			e.m_next != null && (e.m_next.m_prev = e.m_prev),
			e == this.m_bodyList && (this.m_bodyList = e.m_next),
			--this.m_bodyCount
		},
		createBody: function (e) {
			if (this.isLocked() == 1) return null;
			var t = new d.dynamics.B2Body(e, this);
			return t.m_prev = null,
			t.m_next = this.m_bodyList,
			this.m_bodyList != null && (this.m_bodyList.m_prev = t),
			this.m_bodyList = t,
			++this.m_bodyCount,
			t
		},
		getProxyCount: function () {
			return this.m_contactManager.m_broadPhase.getProxyCount()
		},
		validate: function () {
			this.m_contactManager.m_broadPhase.validate()
		},
		setBroadPhase: function (e) {
			var t = this.m_contactManager.m_broadPhase;
			this.m_contactManager.m_broadPhase = e;
			var n = this.m_bodyList;
			while (n != null) {
				var r = n.m_fixtureList;
				while (r != null) r.m_proxy = e.createProxy(t.getFatAABB(r.m_proxy), r),
				r = r.m_next;
				n = n.m_next
			}
		},
		setDebugDraw: function (e) {
			this.m_debugDraw = e
		},
		setContactListener: function (e) {
			this.m_contactManager.m_contactListener = e
		},
		setContactFilter: function (e) {
			this.m_contactManager.m_contactFilter = e
		},
		setDestructionListener: function (e) {
			this.m_destructionListener = e
		},
		__class__: d.dynamics.B2World
	},
	d.dynamics.contacts = {},
	d.dynamics.contacts.B2Contact = function () {
		this.m_nodeA = new d.dynamics.contacts.B2ContactEdge,
		this.m_nodeB = new d.dynamics.contacts.B2ContactEdge,
		this.m_manifold = new d.collision.B2Manifold,
		this.m_oldManifold = new d.collision.B2Manifold
	},
	d.dynamics.contacts.B2Contact.__name__ = !0,
	d.dynamics.contacts.B2Contact.prototype = {
		computeTOI: function (e, t) {
			return d.dynamics.contacts.B2Contact.s_input.proxyA.set(this.m_fixtureA.getShape()),
			d.dynamics.contacts.B2Contact.s_input.proxyB.set(this.m_fixtureB.getShape()),
			d.dynamics.contacts.B2Contact.s_input.sweepA = e,
			d.dynamics.contacts.B2Contact.s_input.sweepB = t,
			d.dynamics.contacts.B2Contact.s_input.tolerance = d.common.B2Settings.b2_linearSlop,
			d.collision.B2TimeOfImpact.timeOfImpact(d.dynamics.contacts.B2Contact.s_input)
		},
		evaluate: function () {},
		update: function (e) {
			var t = this.m_oldManifold;
			this.m_oldManifold = this.m_manifold,
			this.m_manifold = t,
			this.m_flags |= d.dynamics.contacts.B2Contact.e_enabledFlag;
			var n = !1,
			r = (this.m_flags & d.dynamics.contacts.B2Contact.e_touchingFlag) == d.dynamics.contacts.B2Contact.e_touchingFlag,
			i = this.m_fixtureA.m_body,
			s = this.m_fixtureB.m_body,
			o = this.m_fixtureA.m_aabb.testOverlap(this.m_fixtureB.m_aabb);
			if ((this.m_flags & d.dynamics.contacts.B2Contact.e_sensorFlag) != 0) {
				if (o) {
					var u = this.m_fixtureA.getShape(),
					a = this.m_fixtureB.getShape(),
					f = i.getTransform(),
					l = s.getTransform();
					n = d.collision.shapes.B2Shape.testOverlap(u, f, a, l)
				}
				this.m_manifold.m_pointCount = 0
			} else {
				i.getType() != d.dynamics.B2Body.b2_dynamicBody || i.isBullet() || s.getType() != d.dynamics.B2Body.b2_dynamicBody || s.isBullet() ? this.m_flags |= d.dynamics.contacts.B2Contact.e_continuousFlag: this.m_flags &= ~d.dynamics.contacts.B2Contact.e_continuousFlag;
				if (o) {
					this.evaluate(),
					n = this.m_manifold.m_pointCount > 0;
					var c = 0,
					h = this.m_manifold.m_pointCount;
					while (c < h) {
						var p = c++,
						v = this.m_manifold.m_points[p];
						v.m_normalImpulse = 0,
						v.m_tangentImpulse = 0;
						var m = v.m_id,
						g = 0,
						y = this.m_oldManifold.m_pointCount;
						while (g < y) {
							var b = g++,
							w = this.m_oldManifold.m_points[b];
							if (w.m_id.get_key() == m.get_key()) {
								v.m_normalImpulse = w.m_normalImpulse,
								v.m_tangentImpulse = w.m_tangentImpulse;
								break
							}
						}
					}
				} else this.m_manifold.m_pointCount = 0;
				n != r && (i.setAwake(!0), s.setAwake(!0))
			}
			n ? this.m_flags |= d.dynamics.contacts.B2Contact.e_touchingFlag: this.m_flags &= ~d.dynamics.contacts.B2Contact.e_touchingFlag,
			r == 0 && n == 1 && e.beginContact(this),
			r == 1 && n == 0 && e.endContact(this),
			(this.m_flags & d.dynamics.contacts.B2Contact.e_sensorFlag) == 0 && e.preSolve(this, this.m_oldManifold)
		},
		reset: function (e, t) {
			this.m_flags = d.dynamics.contacts.B2Contact.e_enabledFlag;
			if (e == null || t == null) {
				this.m_fixtureA = null,
				this.m_fixtureB = null;
				return
			}
			if (e.isSensor() || t.isSensor()) this.m_flags |= d.dynamics.contacts.B2Contact.e_sensorFlag;
			var n = e.getBody(),
			r = t.getBody();
			if (n.getType() != d.dynamics.B2Body.b2_dynamicBody || n.isBullet() || r.getType() != d.dynamics.B2Body.b2_dynamicBody || r.isBullet()) this.m_flags |= d.dynamics.contacts.B2Contact.e_continuousFlag;
			this.m_fixtureA = e,
			this.m_fixtureB = t,
			this.m_manifold.m_pointCount = 0,
			this.m_prev = null,
			this.m_next = null,
			this.m_nodeA.contact = null,
			this.m_nodeA.prev = null,
			this.m_nodeA.next = null,
			this.m_nodeA.other = null,
			this.m_nodeB.contact = null,
			this.m_nodeB.prev = null,
			this.m_nodeB.next = null,
			this.m_nodeB.other = null
		},
		flagForFiltering: function () {
			this.m_flags |= d.dynamics.contacts.B2Contact.e_filterFlag
		},
		getFixtureB: function () {
			return this.m_fixtureB
		},
		getFixtureA: function () {
			return this.m_fixtureA
		},
		getNext: function () {
			return this.m_next
		},
		isEnabled: function () {
			return (this.m_flags & d.dynamics.contacts.B2Contact.e_enabledFlag) == d.dynamics.contacts.B2Contact.e_enabledFlag
		},
		setEnabled: function (e) {
			e ? this.m_flags |= d.dynamics.contacts.B2Contact.e_enabledFlag: this.m_flags &= ~d.dynamics.contacts.B2Contact.e_enabledFlag
		},
		isSensor: function () {
			return (this.m_flags & d.dynamics.contacts.B2Contact.e_sensorFlag) == d.dynamics.contacts.B2Contact.e_sensorFlag
		},
		setSensor: function (e) {
			e ? this.m_flags |= d.dynamics.contacts.B2Contact.e_sensorFlag: this.m_flags &= ~d.dynamics.contacts.B2Contact.e_sensorFlag
		},
		isContinuous: function () {
			return (this.m_flags & d.dynamics.contacts.B2Contact.e_continuousFlag) == d.dynamics.contacts.B2Contact.e_continuousFlag
		},
		isTouching: function () {
			return (this.m_flags & d.dynamics.contacts.B2Contact.e_touchingFlag) == d.dynamics.contacts.B2Contact.e_touchingFlag
		},
		getWorldManifold: function (e) {
			var t = this.m_fixtureA.getBody(),
			n = this.m_fixtureB.getBody(),
			r = this.m_fixtureA.getShape(),
			i = this.m_fixtureB.getShape();
			e.initialize(this.m_manifold, t.getTransform(), r.m_radius, n.getTransform(), i.m_radius)
		},
		getManifold: function () {
			return this.m_manifold
		},
		__class__: d.dynamics.contacts.B2Contact
	},
	d.dynamics.contacts.B2CircleContact = function () {
		d.dynamics.contacts.B2Contact.call(this)
	},
	d.dynamics.contacts.B2CircleContact.__name__ = !0,
	d.dynamics.contacts.B2CircleContact.create = function (e) {
		return new d.dynamics.contacts.B2CircleContact
	},
	d.dynamics.contacts.B2CircleContact.destroy = function (e, t) {},
	d.dynamics.contacts.B2CircleContact.__super__ = d.dynamics.contacts.B2Contact,
	d.dynamics.contacts.B2CircleContact.prototype = t(d.dynamics.contacts.B2Contact.prototype, {
		evaluate: function () {
			var e = this.m_fixtureA.getBody(),
			t = this.m_fixtureB.getBody();
			d.collision.B2Collision.collideCircles(this.m_manifold, y.Boot.__cast(this.m_fixtureA.getShape(), d.collision.shapes.B2CircleShape), e.m_xf, y.Boot.__cast(this.m_fixtureB.getShape(), d.collision.shapes.B2CircleShape), t.m_xf)
		},
		reset: function (e, t) {
			d.dynamics.contacts.B2Contact.prototype.reset.call(this, e, t)
		},
		__class__: d.dynamics.contacts.B2CircleContact
	}),
	d.dynamics.contacts.B2ContactConstraint = function () {
		this.localPlaneNormal = new d.common.math.B2Vec2,
		this.localPoint = new d.common.math.B2Vec2,
		this.normal = new d.common.math.B2Vec2,
		this.normalMass = new d.common.math.B2Mat22,
		this.K = new d.common.math.B2Mat22,
		this.points = new Array;
		var e = 0,
		t = d.common.B2Settings.b2_maxManifoldPoints;
		while (e < t) {
			var n = e++;
			this.points[n] = new d.dynamics.contacts.B2ContactConstraintPoint
		}
	},
	d.dynamics.contacts.B2ContactConstraint.__name__ = !0,
	d.dynamics.contacts.B2ContactConstraint.prototype = {
		__class__: d.dynamics.contacts.B2ContactConstraint
	},
	d.dynamics.contacts.B2ContactConstraintPoint = function () {
		this.localPoint = new d.common.math.B2Vec2,
		this.rA = new d.common.math.B2Vec2,
		this.rB = new d.common.math.B2Vec2
	},
	d.dynamics.contacts.B2ContactConstraintPoint.__name__ = !0,
	d.dynamics.contacts.B2ContactConstraintPoint.prototype = {
		__class__: d.dynamics.contacts.B2ContactConstraintPoint
	},
	d.dynamics.contacts.B2ContactEdge = function () {},
	d.dynamics.contacts.B2ContactEdge.__name__ = !0,
	d.dynamics.contacts.B2ContactEdge.prototype = {
		__class__: d.dynamics.contacts.B2ContactEdge
	},
	d.dynamics.contacts.B2ContactFactory = function (e) {
		this.m_allocator = e,
		this.initializeRegisters()
	},
	d.dynamics.contacts.B2ContactFactory.__name__ = !0,
	d.dynamics.contacts.B2ContactFactory.prototype = {
		destroy: function (e) {
			e.m_manifold.m_pointCount > 0 && (e.m_fixtureA.m_body.setAwake(!0), e.m_fixtureB.m_body.setAwake(!0));
			var t = e.m_fixtureA.getType(),
			n = e.m_fixtureB.getType(),
			r = this.m_registers[t][n];
			r.poolCount++,
			e.m_next = r.pool,
			r.pool = e;
			var i = r.destroyFcn;
			i(e, this.m_allocator)
		},
		create: function (e, t) {
			var n = e.getType(),
			r = t.getType(),
			i = this.m_registers[n][r],
			s;
			if (i.pool != null) return s = i.pool,
			i.pool = s.m_next,
			i.poolCount--,
			s.reset(e, t),
			s;
			var o = i.createFcn;
			return o != null ? i.primary ? (s = o(this.m_allocator), s.reset(e, t), s) : (s = o(this.m_allocator), s.reset(t, e), s) : null
		},
		initializeRegisters: function () {
			this.m_registers = new Array;
			var e = 0,
			t = d.collision.shapes.B2Shape.e_shapeTypeCount;
			while (e < t) {
				var n = e++;
				this.m_registers[n] = new Array;
				var r = 0,
				i = d.collision.shapes.B2Shape.e_shapeTypeCount;
				while (r < i) {
					var s = r++;
					this.m_registers[n][s] = new d.dynamics.contacts.B2ContactRegister
				}
			}
			this.addType(d.dynamics.contacts.B2CircleContact.create, d.dynamics.contacts.B2CircleContact.destroy, d.collision.shapes.B2Shape.e_circleShape, d.collision.shapes.B2Shape.e_circleShape),
			this.addType(d.dynamics.contacts.B2PolyAndCircleContact.create, d.dynamics.contacts.B2PolyAndCircleContact.destroy, d.collision.shapes.B2Shape.e_polygonShape, d.collision.shapes.B2Shape.e_circleShape),
			this.addType(d.dynamics.contacts.B2PolygonContact.create, d.dynamics.contacts.B2PolygonContact.destroy, d.collision.shapes.B2Shape.e_polygonShape, d.collision.shapes.B2Shape.e_polygonShape),
			this.addType(d.dynamics.contacts.B2EdgeAndCircleContact.create, d.dynamics.contacts.B2EdgeAndCircleContact.destroy, d.collision.shapes.B2Shape.e_edgeShape, d.collision.shapes.B2Shape.e_circleShape),
			this.addType(d.dynamics.contacts.B2PolyAndEdgeContact.create, d.dynamics.contacts.B2PolyAndEdgeContact.destroy, d.collision.shapes.B2Shape.e_polygonShape, d.collision.shapes.B2Shape.e_edgeShape)
		},
		addType: function (e, t, n, r) {
			this.m_registers[n][r].createFcn = e,
			this.m_registers[n][r].destroyFcn = t,
			this.m_registers[n][r].primary = !0,
			n != r && (this.m_registers[r][n].createFcn = e, this.m_registers[r][n].destroyFcn = t, this.m_registers[r][n].primary = !1)
		},
		__class__: d.dynamics.contacts.B2ContactFactory
	},
	d.dynamics.contacts.B2ContactRegister = function () {},
	d.dynamics.contacts.B2ContactRegister.__name__ = !0,
	d.dynamics.contacts.B2ContactRegister.prototype = {
		__class__: d.dynamics.contacts.B2ContactRegister
	},
	d.dynamics.contacts.B2PositionSolverManifold = function () {
		this.m_normal = new d.common.math.B2Vec2,
		this.m_separations = new Array,
		this.m_points = new Array;
		var e = 0,
		t = d.common.B2Settings.b2_maxManifoldPoints;
		while (e < t) {
			var n = e++;
			this.m_points[n] = new d.common.math.B2Vec2
		}
	},
	d.dynamics.contacts.B2PositionSolverManifold.__name__ = !0,
	d.dynamics.contacts.B2PositionSolverManifold.prototype = {
		initialize: function (e) {
			d.common.B2Settings.b2Assert(e.pointCount > 0);
			var t, n, r, i, s, o, u;
			switch (e.type) {
			case d.collision.B2Manifold.e_circles:
				i = e.bodyA.m_xf.R,
				s = e.localPoint;
				var a = e.bodyA.m_xf.position.x + (i.col1.x * s.x + i.col2.x * s.y),
				f = e.bodyA.m_xf.position.y + (i.col1.y * s.x + i.col2.y * s.y);
				i = e.bodyB.m_xf.R,
				s = e.points[0].localPoint;
				var l = e.bodyB.m_xf.position.x + (i.col1.x * s.x + i.col2.x * s.y),
				c = e.bodyB.m_xf.position.y + (i.col1.y * s.x + i.col2.y * s.y),
				h = l - a,
				p = c - f,
				v = h * h + p * p;
				if (v > 0) {
					var m = Math.sqrt(v);
					this.m_normal.x = h / m,
					this.m_normal.y = p / m
				} else this.m_normal.x = 1,
				this.m_normal.y = 0;
				this.m_points[0].x = .5 * (a + l),
				this.m_points[0].y = .5 * (f + c),
				this.m_separations[0] = h * this.m_normal.x + p * this.m_normal.y - e.radius;
				break;
			case d.collision.B2Manifold.e_faceA:
				i = e.bodyA.m_xf.R,
				s = e.localPlaneNormal,
				this.m_normal.x = i.col1.x * s.x + i.col2.x * s.y,
				this.m_normal.y = i.col1.y * s.x + i.col2.y * s.y,
				i = e.bodyA.m_xf.R,
				s = e.localPoint,
				o = e.bodyA.m_xf.position.x + (i.col1.x * s.x + i.col2.x * s.y),
				u = e.bodyA.m_xf.position.y + (i.col1.y * s.x + i.col2.y * s.y),
				i = e.bodyB.m_xf.R;
				var g = 0,
				y = e.pointCount;
				while (g < y) {
					var b = g++;
					s = e.points[b].localPoint,
					n = e.bodyB.m_xf.position.x + (i.col1.x * s.x + i.col2.x * s.y),
					r = e.bodyB.m_xf.position.y + (i.col1.y * s.x + i.col2.y * s.y),
					this.m_separations[b] = (n - o) * this.m_normal.x + (r - u) * this.m_normal.y - e.radius,
					this.m_points[b].x = n,
					this.m_points[b].y = r
				}
				break;
			case d.collision.B2Manifold.e_faceB:
				i = e.bodyB.m_xf.R,
				s = e.localPlaneNormal,
				this.m_normal.x = i.col1.x * s.x + i.col2.x * s.y,
				this.m_normal.y = i.col1.y * s.x + i.col2.y * s.y,
				i = e.bodyB.m_xf.R,
				s = e.localPoint,
				o = e.bodyB.m_xf.position.x + (i.col1.x * s.x + i.col2.x * s.y),
				u = e.bodyB.m_xf.position.y + (i.col1.y * s.x + i.col2.y * s.y),
				i = e.bodyA.m_xf.R;
				var g = 0,
				y = e.pointCount;
				while (g < y) {
					var b = g++;
					s = e.points[b].localPoint,
					n = e.bodyA.m_xf.position.x + (i.col1.x * s.x + i.col2.x * s.y),
					r = e.bodyA.m_xf.position.y + (i.col1.y * s.x + i.col2.y * s.y),
					this.m_separations[b] = (n - o) * this.m_normal.x + (r - u) * this.m_normal.y - e.radius,
					this.m_points[b].set(n, r)
				}
				this.m_normal.x *= -1,
				this.m_normal.y *= -1
			}
		},
		__class__: d.dynamics.contacts.B2PositionSolverManifold
	},
	d.dynamics.contacts.B2ContactSolver = function () {
		this.m_step = new d.dynamics.B2TimeStep,
		this.m_constraints = new Array
	},
	d.dynamics.contacts.B2ContactSolver.__name__ = !0,
	d.dynamics.contacts.B2ContactSolver.prototype = {
		solvePositionConstraints: function (e) {
			var t = 0,
			n = 0,
			r = this.m_constraintCount;
			while (n < r) {
				var i = n++,
				s = this.m_constraints[i],
				o = s.bodyA,
				u = s.bodyB,
				a = o.m_mass * o.m_invMass,
				f = o.m_mass * o.m_invI,
				l = u.m_mass * u.m_invMass,
				c = u.m_mass * u.m_invI;
				d.dynamics.contacts.B2ContactSolver.s_psm.initialize(s);
				var h = d.dynamics.contacts.B2ContactSolver.s_psm.m_normal,
				p = 0,
				v = s.pointCount;
				while (p < v) {
					var m = p++,
					g = s.points[m],
					y = d.dynamics.contacts.B2ContactSolver.s_psm.m_points[m],
					b = d.dynamics.contacts.B2ContactSolver.s_psm.m_separations[m],
					w = y.x - o.m_sweep.c.x,
					E = y.y - o.m_sweep.c.y,
					S = y.x - u.m_sweep.c.x,
					x = y.y - u.m_sweep.c.y;
					t = t < b ? t: b;
					var T = d.common.math.B2Math.clamp(e * (b + d.common.B2Settings.b2_linearSlop), -d.common.B2Settings.b2_maxLinearCorrection, 0),
					N = -g.equalizedMass * T,
					C = N * h.x,
					k = N * h.y;
					o.m_sweep.c.x -= a * C,
					o.m_sweep.c.y -= a * k,
					o.m_sweep.a -= f * (w * k - E * C),
					o.synchronizeTransform(),
					u.m_sweep.c.x += l * C,
					u.m_sweep.c.y += l * k,
					u.m_sweep.a += c * (S * k - x * C),
					u.synchronizeTransform()
				}
			}
			return t > -1.5 * d.common.B2Settings.b2_linearSlop
		},
		finalizeVelocityConstraints: function () {
			var e = 0,
			t = this.m_constraintCount;
			while (e < t) {
				var n = e++,
				r = this.m_constraints[n],
				i = r.manifold,
				s = 0,
				o = r.pointCount;
				while (s < o) {
					var u = s++,
					a = i.m_points[u],
					f = r.points[u];
					a.m_normalImpulse = f.normalImpulse,
					a.m_tangentImpulse = f.tangentImpulse
				}
			}
		},
		solveVelocityConstraints: function () {
			var e, t, n, r, i, s, o, u, a, f, l, c, h, p, v, m, g, y, b, w, E, S, x, T = 0,
			N = this.m_constraintCount;
			while (T < N) {
				var C = T++,
				k = this.m_constraints[C],
				L = k.bodyA,
				A = k.bodyB,
				O = L.m_angularVelocity,
				M = A.m_angularVelocity,
				_ = L.m_linearVelocity,
				D = A.m_linearVelocity,
				P = L.m_invMass,
				H = L.m_invI,
				B = A.m_invMass,
				j = A.m_invI,
				F = k.normal.x,
				I = k.normal.y,
				q = I,
				R = -F,
				U = k.friction,
				z,
				W = 0,
				X = k.pointCount;
				while (W < X) {
					var V = W++;
					t = k.points[V],
					o = D.x - M * t.rB.y - _.x + O * t.rA.y,
					u = D.y + M * t.rB.x - _.y - O * t.rA.x,
					f = o * q + u * R,
					l = t.tangentMass * -f,
					c = U * t.normalImpulse,
					h = d.common.math.B2Math.clamp(t.tangentImpulse + l, -c, c),
					l = h - t.tangentImpulse,
					p = l * q,
					v = l * R,
					_.x -= P * p,
					_.y -= P * v,
					O -= H * (t.rA.x * v - t.rA.y * p),
					D.x += B * p,
					D.y += B * v,
					M += j * (t.rB.x * v - t.rB.y * p),
					t.tangentImpulse = h
				}
				var $ = k.pointCount;
				if (k.pointCount == 1) t = k.points[0],
				o = D.x + -M * t.rB.y - _.x - -O * t.rA.y,
				u = D.y + M * t.rB.x - _.y - O * t.rA.x,
				a = o * F + u * I,
				l = -t.normalMass * (a - t.velocityBias),
				h = t.normalImpulse + l,
				h = h > 0 ? h: 0,
				l = h - t.normalImpulse,
				p = l * F,
				v = l * I,
				_.x -= P * p,
				_.y -= P * v,
				O -= H * (t.rA.x * v - t.rA.y * p),
				D.x += B * p,
				D.y += B * v,
				M += j * (t.rB.x * v - t.rB.y * p),
				t.normalImpulse = h;
				else {
					var J = k.points[0],
					K = k.points[1],
					Q = J.normalImpulse,
					G = K.normalImpulse,
					Y = D.x - M * J.rB.y - _.x + O * J.rA.y,
					Z = D.y + M * J.rB.x - _.y - O * J.rA.x,
					et = D.x - M * K.rB.y - _.x + O * K.rA.y,
					tt = D.y + M * K.rB.x - _.y - O * K.rA.x,
					nt = Y * F + Z * I,
					rt = et * F + tt * I,
					it = nt - J.velocityBias,
					st = rt - K.velocityBias;
					S = k.K,
					it -= S.col1.x * Q + S.col2.x * G,
					st -= S.col1.y * Q + S.col2.y * G;
					var ot = .001,
					X = 0;
					while (X < 1) {
						var ut = X++;
						S = k.normalMass;
						var at = -(S.col1.x * it + S.col2.x * st),
						ft = -(S.col1.y * it + S.col2.y * st);
						if (at >= 0 && ft >= 0) {
							m = at - Q,
							g = ft - G,
							y = m * F,
							b = m * I,
							w = g * F,
							E = g * I,
							_.x -= P * (y + w),
							_.y -= P * (b + E),
							O -= H * (J.rA.x * b - J.rA.y * y + K.rA.x * E - K.rA.y * w),
							D.x += B * (y + w),
							D.y += B * (b + E),
							M += j * (J.rB.x * b - J.rB.y * y + K.rB.x * E - K.rB.y * w),
							J.normalImpulse = at,
							K.normalImpulse = ft;
							break
						}
						at = -J.normalMass * it,
						ft = 0,
						nt = 0,
						rt = k.K.col1.y * at + st;
						if (at >= 0 && rt >= 0) {
							m = at - Q,
							g = ft - G,
							y = m * F,
							b = m * I,
							w = g * F,
							E = g * I,
							_.x -= P * (y + w),
							_.y -= P * (b + E),
							O -= H * (J.rA.x * b - J.rA.y * y + K.rA.x * E - K.rA.y * w),
							D.x += B * (y + w),
							D.y += B * (b + E),
							M += j * (J.rB.x * b - J.rB.y * y + K.rB.x * E - K.rB.y * w),
							J.normalImpulse = at,
							K.normalImpulse = ft;
							break
						}
						at = 0,
						ft = -K.normalMass * st,
						nt = k.K.col2.x * ft + it,
						rt = 0;
						if (ft >= 0 && nt >= 0) {
							m = at - Q,
							g = ft - G,
							y = m * F,
							b = m * I,
							w = g * F,
							E = g * I,
							_.x -= P * (y + w),
							_.y -= P * (b + E),
							O -= H * (J.rA.x * b - J.rA.y * y + K.rA.x * E - K.rA.y * w),
							D.x += B * (y + w),
							D.y += B * (b + E),
							M += j * (J.rB.x * b - J.rB.y * y + K.rB.x * E - K.rB.y * w),
							J.normalImpulse = at,
							K.normalImpulse = ft;
							break
						}
						at = 0,
						ft = 0,
						nt = it,
						rt = st;
						if (nt >= 0 && rt >= 0) {
							m = at - Q,
							g = ft - G,
							y = m * F,
							b = m * I,
							w = g * F,
							E = g * I,
							_.x -= P * (y + w),
							_.y -= P * (b + E),
							O -= H * (J.rA.x * b - J.rA.y * y + K.rA.x * E - K.rA.y * w),
							D.x += B * (y + w),
							D.y += B * (b + E),
							M += j * (J.rB.x * b - J.rB.y * y + K.rB.x * E - K.rB.y * w),
							J.normalImpulse = at,
							K.normalImpulse = ft;
							break
						}
						break
					}
				}
				L.m_angularVelocity = O,
				A.m_angularVelocity = M
			}
		},
		initVelocityConstraints: function (e) {
			var t, n, r, i = 0,
			s = this.m_constraintCount;
			while (i < s) {
				var o = i++,
				u = this.m_constraints[o],
				a = u.bodyA,
				f = u.bodyB,
				l = a.m_invMass,
				c = a.m_invI,
				h = f.m_invMass,
				p = f.m_invI,
				d = u.normal.x,
				v = u.normal.y,
				m = v,
				g = -d,
				y,
				b,
				w;
				if (e.warmStarting) {
					w = u.pointCount;
					var E = 0;
					while (E < w) {
						var S = E++,
						x = u.points[S];
						x.normalImpulse *= e.dtRatio,
						x.tangentImpulse *= e.dtRatio;
						var T = x.normalImpulse * d + x.tangentImpulse * m,
						N = x.normalImpulse * v + x.tangentImpulse * g;
						a.m_angularVelocity -= c * (x.rA.x * N - x.rA.y * T),
						a.m_linearVelocity.x -= l * T,
						a.m_linearVelocity.y -= l * N,
						f.m_angularVelocity += p * (x.rB.x * N - x.rB.y * T),
						f.m_linearVelocity.x += h * T,
						f.m_linearVelocity.y += h * N
					}
				} else {
					w = u.pointCount;
					var E = 0;
					while (E < w) {
						var S = E++,
						C = u.points[S];
						C.normalImpulse = 0,
						C.tangentImpulse = 0
					}
				}
			}
		},
		initialize: function (e, t, n, r) {
			var i;
			this.m_step.set(e),
			this.m_allocator = r;
			var s, o, u;
			this.m_constraintCount = n;
			while (this.m_constraints.length < this.m_constraintCount) this.m_constraints[this.m_constraints.length] = new d.dynamics.contacts.B2ContactConstraint;
			var a = 0;
			while (a < n) {
				var f = a++;
				i = t[f];
				var l = i.m_fixtureA,
				c = i.m_fixtureB,
				h = l.m_shape,
				p = c.m_shape,
				v = h.m_radius,
				m = p.m_radius,
				g = l.m_body,
				y = c.m_body,
				b = i.getManifold(),
				w = d.common.B2Settings.b2MixFriction(l.getFriction(), c.getFriction()),
				E = d.common.B2Settings.b2MixRestitution(l.getRestitution(), c.getRestitution()),
				S = g.m_linearVelocity.x,
				x = g.m_linearVelocity.y,
				T = y.m_linearVelocity.x,
				N = y.m_linearVelocity.y,
				C = g.m_angularVelocity,
				k = y.m_angularVelocity;
				d.common.B2Settings.b2Assert(b.m_pointCount > 0),
				d.dynamics.contacts.B2ContactSolver.s_worldManifold.initialize(b, g.m_xf, v, y.m_xf, m);
				var L = d.dynamics.contacts.B2ContactSolver.s_worldManifold.m_normal.x,
				A = d.dynamics.contacts.B2ContactSolver.s_worldManifold.m_normal.y,
				O = this.m_constraints[f];
				O.bodyA = g,
				O.bodyB = y,
				O.manifold = b,
				O.normal.x = L,
				O.normal.y = A,
				O.pointCount = b.m_pointCount,
				O.friction = w,
				O.restitution = E,
				O.localPlaneNormal.x = b.m_localPlaneNormal.x,
				O.localPlaneNormal.y = b.m_localPlaneNormal.y,
				O.localPoint.x = b.m_localPoint.x,
				O.localPoint.y = b.m_localPoint.y,
				O.radius = v + m,
				O.type = b.m_type;
				var M = 0,
				_ = O.pointCount;
				while (M < _) {
					var D = M++,
					P = b.m_points[D],
					H = O.points[D];
					H.normalImpulse = P.m_normalImpulse,
					H.tangentImpulse = P.m_tangentImpulse,
					H.localPoint.setV(P.m_localPoint);
					var B = H.rA.x = d.dynamics.contacts.B2ContactSolver.s_worldManifold.m_points[D].x - g.m_sweep.c.x,
					j = H.rA.y = d.dynamics.contacts.B2ContactSolver.s_worldManifold.m_points[D].y - g.m_sweep.c.y,
					F = H.rB.x = d.dynamics.contacts.B2ContactSolver.s_worldManifold.m_points[D].x - y.m_sweep.c.x,
					I = H.rB.y = d.dynamics.contacts.B2ContactSolver.s_worldManifold.m_points[D].y - y.m_sweep.c.y,
					q = B * A - j * L,
					R = F * A - I * L;
					q *= q,
					R *= R;
					var U = g.m_invMass + y.m_invMass + g.m_invI * q + y.m_invI * R;
					H.normalMass = 1 / U;
					var z = g.m_mass * g.m_invMass + y.m_mass * y.m_invMass;
					z += g.m_mass * g.m_invI * q + y.m_mass * y.m_invI * R,
					H.equalizedMass = 1 / z;
					var W = A,
					X = -L,
					V = B * X - j * W,
					$ = F * X - I * W;
					V *= V,
					$ *= $;
					var J = g.m_invMass + y.m_invMass + g.m_invI * V + y.m_invI * $;
					H.tangentMass = 1 / J,
					H.velocityBias = 0;
					var K = T + -k * I - S - -C * j,
					Q = N + k * F - x - C * B,
					G = O.normal.x * K + O.normal.y * Q;
					G < -d.common.B2Settings.b2_velocityThreshold && (H.velocityBias += -O.restitution * G)
				}
				if (O.pointCount == 2) {
					var Y = O.points[0],
					Z = O.points[1],
					et = g.m_invMass,
					tt = g.m_invI,
					nt = y.m_invMass,
					rt = y.m_invI,
					it = Y.rA.x * A - Y.rA.y * L,
					st = Y.rB.x * A - Y.rB.y * L,
					ot = Z.rA.x * A - Z.rA.y * L,
					ut = Z.rB.x * A - Z.rB.y * L,
					at = et + nt + tt * it * it + rt * st * st,
					ft = et + nt + tt * ot * ot + rt * ut * ut,
					lt = et + nt + tt * it * ot + rt * st * ut,
					ct = 100;
					at * at < ct * (at * ft - lt * lt) ? (O.K.col1.set(at, lt), O.K.col2.set(lt, ft), O.K.getInverse(O.normalMass)) : O.pointCount = 1
				}
			}
		},
		__class__: d.dynamics.contacts.B2ContactSolver
	},
	d.dynamics.contacts.B2EdgeAndCircleContact = function () {
		d.dynamics.contacts.B2Contact.call(this)
	},
	d.dynamics.contacts.B2EdgeAndCircleContact.__name__ = !0,
	d.dynamics.contacts.B2EdgeAndCircleContact.create = function (e) {
		return new d.dynamics.contacts.B2EdgeAndCircleContact
	},
	d.dynamics.contacts.B2EdgeAndCircleContact.destroy = function (e, t) {},
	d.dynamics.contacts.B2EdgeAndCircleContact.__super__ = d.dynamics.contacts.B2Contact,
	d.dynamics.contacts.B2EdgeAndCircleContact.prototype = t(d.dynamics.contacts.B2Contact.prototype, {
		b2CollideEdgeAndCircle: function (e, t, n, r, i) {},
		evaluate: function () {
			var e = this.m_fixtureA.getBody(),
			t = this.m_fixtureB.getBody();
			this.b2CollideEdgeAndCircle(this.m_manifold, y.Boot.__cast(this.m_fixtureA.getShape(), d.collision.shapes.B2EdgeShape), e.m_xf, y.Boot.__cast(this.m_fixtureB.getShape(), d.collision.shapes.B2CircleShape), t.m_xf)
		},
		reset: function (e, t) {
			d.dynamics.contacts.B2Contact.prototype.reset.call(this, e, t)
		},
		__class__: d.dynamics.contacts.B2EdgeAndCircleContact
	}),
	d.dynamics.contacts.B2PolyAndCircleContact = function () {
		d.dynamics.contacts.B2Contact.call(this)
	},
	d.dynamics.contacts.B2PolyAndCircleContact.__name__ = !0,
	d.dynamics.contacts.B2PolyAndCircleContact.create = function (e) {
		return new d.dynamics.contacts.B2PolyAndCircleContact
	},
	d.dynamics.contacts.B2PolyAndCircleContact.destroy = function (e, t) {},
	d.dynamics.contacts.B2PolyAndCircleContact.__super__ = d.dynamics.contacts.B2Contact,
	d.dynamics.contacts.B2PolyAndCircleContact.prototype = t(d.dynamics.contacts.B2Contact.prototype, {
		evaluate: function () {
			var e = this.m_fixtureA.m_body,
			t = this.m_fixtureB.m_body;
			d.collision.B2Collision.collidePolygonAndCircle(this.m_manifold, y.Boot.__cast(this.m_fixtureA.getShape(), d.collision.shapes.B2PolygonShape), e.m_xf, y.Boot.__cast(this.m_fixtureB.getShape(), d.collision.shapes.B2CircleShape), t.m_xf)
		},
		reset: function (e, t) {
			d.dynamics.contacts.B2Contact.prototype.reset.call(this, e, t),
			d.common.B2Settings.b2Assert(e.getType() == d.collision.shapes.B2Shape.e_polygonShape),
			d.common.B2Settings.b2Assert(t.getType() == d.collision.shapes.B2Shape.e_circleShape)
		},
		__class__: d.dynamics.contacts.B2PolyAndCircleContact
	}),
	d.dynamics.contacts.B2PolyAndEdgeContact = function () {
		d.dynamics.contacts.B2Contact.call(this)
	},
	d.dynamics.contacts.B2PolyAndEdgeContact.__name__ = !0,
	d.dynamics.contacts.B2PolyAndEdgeContact.create = function (e) {
		return new d.dynamics.contacts.B2PolyAndEdgeContact
	},
	d.dynamics.contacts.B2PolyAndEdgeContact.destroy = function (e, t) {},
	d.dynamics.contacts.B2PolyAndEdgeContact.__super__ = d.dynamics.contacts.B2Contact,
	d.dynamics.contacts.B2PolyAndEdgeContact.prototype = t(d.dynamics.contacts.B2Contact.prototype, {
		b2CollidePolyAndEdge: function (e, t, n, r, i) {},
		evaluate: function () {
			var e = this.m_fixtureA.getBody(),
			t = this.m_fixtureB.getBody();
			this.b2CollidePolyAndEdge(this.m_manifold, y.Boot.__cast(this.m_fixtureA.getShape(), d.collision.shapes.B2PolygonShape), e.m_xf, y.Boot.__cast(this.m_fixtureB.getShape(), d.collision.shapes.B2EdgeShape), t.m_xf)
		},
		reset: function (e, t) {
			d.dynamics.contacts.B2Contact.prototype.reset.call(this, e, t),
			d.common.B2Settings.b2Assert(e.getType() == d.collision.shapes.B2Shape.e_polygonShape),
			d.common.B2Settings.b2Assert(t.getType() == d.collision.shapes.B2Shape.e_edgeShape)
		},
		__class__: d.dynamics.contacts.B2PolyAndEdgeContact
	}),
	d.dynamics.contacts.B2PolygonContact = function () {
		d.dynamics.contacts.B2Contact.call(this)
	},
	d.dynamics.contacts.B2PolygonContact.__name__ = !0,
	d.dynamics.contacts.B2PolygonContact.create = function (e) {
		return new d.dynamics.contacts.B2PolygonContact
	},
	d.dynamics.contacts.B2PolygonContact.destroy = function (e, t) {},
	d.dynamics.contacts.B2PolygonContact.__super__ = d.dynamics.contacts.B2Contact,
	d.dynamics.contacts.B2PolygonContact.prototype = t(d.dynamics.contacts.B2Contact.prototype, {
		evaluate: function () {
			var e = this.m_fixtureA.getBody(),
			t = this.m_fixtureB.getBody();
			d.collision.B2Collision.collidePolygons(this.m_manifold, y.Boot.__cast(this.m_fixtureA.getShape(), d.collision.shapes.B2PolygonShape), e.m_xf, y.Boot.__cast(this.m_fixtureB.getShape(), d.collision.shapes.B2PolygonShape), t.m_xf)
		},
		reset: function (e, t) {
			d.dynamics.contacts.B2Contact.prototype.reset.call(this, e, t)
		},
		__class__: d.dynamics.contacts.B2PolygonContact
	}),
	d.dynamics.controllers = {},
	d.dynamics.controllers.B2Controller = function () {},
	d.dynamics.controllers.B2Controller.__name__ = !0,
	d.dynamics.controllers.B2Controller.prototype = {
		getBodyList: function () {
			return this.m_bodyList
		},
		getWorld: function () {
			return this.m_world
		},
		getNext: function () {
			return this.m_next
		},
		clear: function () {
			while (this.m_bodyList != null) this.removeBody(this.m_bodyList.body)
		},
		removeBody: function (e) {
			var t = e.m_controllerList;
			while (t != null && t.controller != this) t = t.nextController;
			t.prevBody != null && (t.prevBody.nextBody = t.nextBody),
			t.nextBody != null && (t.nextBody.prevBody = t.prevBody),
			t.nextController != null && (t.nextController.prevController = t.prevController),
			t.prevController != null && (t.prevController.nextController = t.nextController),
			this.m_bodyList == t && (this.m_bodyList = t.nextBody),
			e.m_controllerList == t && (e.m_controllerList = t.nextController),
			e.m_controllerCount--,
			this.m_bodyCount--
		},
		addBody: function (e) {
			var t = new d.dynamics.controllers.B2ControllerEdge;
			t.controller = this,
			t.body = e,
			t.nextBody = this.m_bodyList,
			t.prevBody = null,
			this.m_bodyList = t,
			t.nextBody != null && (t.nextBody.prevBody = t),
			this.m_bodyCount++,
			t.nextController = e.m_controllerList,
			t.prevController = null,
			e.m_controllerList = t,
			t.nextController != null && (t.nextController.prevController = t),
			e.m_controllerCount++
		},
		draw: function (e) {},
		step: function (e) {},
		__class__: d.dynamics.controllers.B2Controller
	},
	d.dynamics.controllers.B2ControllerEdge = function () {},
	d.dynamics.controllers.B2ControllerEdge.__name__ = !0,
	d.dynamics.controllers.B2ControllerEdge.prototype = {
		__class__: d.dynamics.controllers.B2ControllerEdge
	},
	d.dynamics.joints = {},
	d.dynamics.joints.B2Joint = function (e) {
		this.m_edgeA = new d.dynamics.joints.B2JointEdge,
		this.m_edgeB = new d.dynamics.joints.B2JointEdge,
		this.m_localCenterA = new d.common.math.B2Vec2,
		this.m_localCenterB = new d.common.math.B2Vec2,
		d.common.B2Settings.b2Assert(e.bodyA != e.bodyB),
		this.m_type = e.type,
		this.m_prev = null,
		this.m_next = null,
		this.m_bodyA = e.bodyA,
		this.m_bodyB = e.bodyB,
		this.m_collideConnected = e.collideConnected,
		this.m_islandFlag = !1,
		this.m_userData = e.userData
	},
	d.dynamics.joints.B2Joint.__name__ = !0,
	d.dynamics.joints.B2Joint.create = function (e, t) {
		var n = null;
		return e.type == d.dynamics.joints.B2Joint.e_distanceJoint ? n = new d.dynamics.joints.B2DistanceJoint(y.Boot.__cast(e, d.dynamics.joints.B2DistanceJointDef)) : e.type == d.dynamics.joints.B2Joint.e_mouseJoint ? n = new d.dynamics.joints.B2MouseJoint(y.Boot.__cast(e, d.dynamics.joints.B2MouseJointDef)) : e.type == d.dynamics.joints.B2Joint.e_prismaticJoint ? n = new d.dynamics.joints.B2PrismaticJoint(y.Boot.__cast(e, d.dynamics.joints.B2PrismaticJointDef)) : e.type == d.dynamics.joints.B2Joint.e_revoluteJoint ? n = new d.dynamics.joints.B2RevoluteJoint(y.Boot.__cast(e, d.dynamics.joints.B2RevoluteJointDef)) : e.type == d.dynamics.joints.B2Joint.e_pulleyJoint ? n = new d.dynamics.joints.B2PulleyJoint(y.Boot.__cast(e, d.dynamics.joints.B2PulleyJointDef)) : e.type == d.dynamics.joints.B2Joint.e_gearJoint ? n = new d.dynamics.joints.B2GearJoint(y.Boot.__cast(e, d.dynamics.joints.B2GearJointDef)) : e.type == d.dynamics.joints.B2Joint.e_lineJoint ? n = new d.dynamics.joints.B2LineJoint(y.Boot.__cast(e, d.dynamics.joints.B2LineJointDef)) : e.type == d.dynamics.joints.B2Joint.e_weldJoint ? n = new d.dynamics.joints.B2WeldJoint(y.Boot.__cast(e, d.dynamics.joints.B2WeldJointDef)) : e.type == d.dynamics.joints.B2Joint.e_frictionJoint && (n = new d.dynamics.joints.B2FrictionJoint(y.Boot.__cast(e, d.dynamics.joints.B2FrictionJointDef))),
		n
	},
	d.dynamics.joints.B2Joint.destroy = function (e, t) {},
	d.dynamics.joints.B2Joint.prototype = {
		solvePositionConstraints: function (e) {
			return ! 1
		},
		finalizeVelocityConstraints: function () {},
		solveVelocityConstraints: function (e) {},
		initVelocityConstraints: function (e) {},
		isActive: function () {
			return this.m_bodyA.isActive() && this.m_bodyB.isActive()
		},
		setUserData: function (e) {
			this.m_userData = e
		},
		getUserData: function () {
			return this.m_userData
		},
		getNext: function () {
			return this.m_next
		},
		getBodyB: function () {
			return this.m_bodyB
		},
		getBodyA: function () {
			return this.m_bodyA
		},
		getReactionTorque: function (e) {
			return 0
		},
		getReactionForce: function (e) {
			return null
		},
		getAnchorB: function () {
			return null
		},
		getAnchorA: function () {
			return null
		},
		getType: function () {
			return this.m_type
		},
		__class__: d.dynamics.joints.B2Joint
	},
	d.dynamics.joints.B2DistanceJoint = function (e) {
		d.dynamics.joints.B2Joint.call(this, e),
		this.m_localAnchor1 = new d.common.math.B2Vec2,
		this.m_localAnchor2 = new d.common.math.B2Vec2,
		this.m_u = new d.common.math.B2Vec2;
		var t, n, r;
		this.m_localAnchor1.setV(e.localAnchorA),
		this.m_localAnchor2.setV(e.localAnchorB),
		this.m_length = e.length,
		this.m_frequencyHz = e.frequencyHz,
		this.m_dampingRatio = e.dampingRatio,
		this.m_impulse = 0,
		this.m_gamma = 0,
		this.m_bias = 0
	},
	d.dynamics.joints.B2DistanceJoint.__name__ = !0,
	d.dynamics.joints.B2DistanceJoint.__super__ = d.dynamics.joints.B2Joint,
	d.dynamics.joints.B2DistanceJoint.prototype = t(d.dynamics.joints.B2Joint.prototype, {
		solvePositionConstraints: function (e) {
			var t;
			if (this.m_frequencyHz > 0) return ! 0;
			var n = this.m_bodyA,
			r = this.m_bodyB;
			t = n.m_xf.R;
			var i = this.m_localAnchor1.x - n.m_sweep.localCenter.x,
			s = this.m_localAnchor1.y - n.m_sweep.localCenter.y,
			o = t.col1.x * i + t.col2.x * s;
			s = t.col1.y * i + t.col2.y * s,
			i = o,
			t = r.m_xf.R;
			var u = this.m_localAnchor2.x - r.m_sweep.localCenter.x,
			a = this.m_localAnchor2.y - r.m_sweep.localCenter.y;
			o = t.col1.x * u + t.col2.x * a,
			a = t.col1.y * u + t.col2.y * a,
			u = o;
			var f = r.m_sweep.c.x + u - n.m_sweep.c.x - i,
			l = r.m_sweep.c.y + a - n.m_sweep.c.y - s,
			c = Math.sqrt(f * f + l * l);
			f /= c,
			l /= c;
			var h = c - this.m_length;
			h = d.common.math.B2Math.clamp(h, -d.common.B2Settings.b2_maxLinearCorrection, d.common.B2Settings.b2_maxLinearCorrection);
			var p = -this.m_mass * h;
			this.m_u.set(f, l);
			var v = p * this.m_u.x,
			m = p * this.m_u.y;
			return n.m_sweep.c.x -= n.m_invMass * v,
			n.m_sweep.c.y -= n.m_invMass * m,
			n.m_sweep.a -= n.m_invI * (i * m - s * v),
			r.m_sweep.c.x += r.m_invMass * v,
			r.m_sweep.c.y += r.m_invMass * m,
			r.m_sweep.a += r.m_invI * (u * m - a * v),
			n.synchronizeTransform(),
			r.synchronizeTransform(),
			d.common.math.B2Math.abs(h) < d.common.B2Settings.b2_linearSlop
		},
		solveVelocityConstraints: function (e) {
			var t, n = this.m_bodyA,
			r = this.m_bodyB;
			t = n.m_xf.R;
			var i = this.m_localAnchor1.x - n.m_sweep.localCenter.x,
			s = this.m_localAnchor1.y - n.m_sweep.localCenter.y,
			o = t.col1.x * i + t.col2.x * s;
			s = t.col1.y * i + t.col2.y * s,
			i = o,
			t = r.m_xf.R;
			var u = this.m_localAnchor2.x - r.m_sweep.localCenter.x,
			a = this.m_localAnchor2.y - r.m_sweep.localCenter.y;
			o = t.col1.x * u + t.col2.x * a,
			a = t.col1.y * u + t.col2.y * a,
			u = o;
			var f = n.m_linearVelocity.x + -n.m_angularVelocity * s,
			l = n.m_linearVelocity.y + n.m_angularVelocity * i,
			c = r.m_linearVelocity.x + -r.m_angularVelocity * a,
			h = r.m_linearVelocity.y + r.m_angularVelocity * u,
			p = this.m_u.x * (c - f) + this.m_u.y * (h - l),
			d = -this.m_mass * (p + this.m_bias + this.m_gamma * this.m_impulse);
			this.m_impulse += d;
			var v = d * this.m_u.x,
			m = d * this.m_u.y;
			n.m_linearVelocity.x -= n.m_invMass * v,
			n.m_linearVelocity.y -= n.m_invMass * m,
			n.m_angularVelocity -= n.m_invI * (i * m - s * v),
			r.m_linearVelocity.x += r.m_invMass * v,
			r.m_linearVelocity.y += r.m_invMass * m,
			r.m_angularVelocity += r.m_invI * (u * m - a * v)
		},
		initVelocityConstraints: function (e) {
			var t, n, r = this.m_bodyA,
			i = this.m_bodyB;
			t = r.m_xf.R;
			var s = this.m_localAnchor1.x - r.m_sweep.localCenter.x,
			o = this.m_localAnchor1.y - r.m_sweep.localCenter.y;
			n = t.col1.x * s + t.col2.x * o,
			o = t.col1.y * s + t.col2.y * o,
			s = n,
			t = i.m_xf.R;
			var u = this.m_localAnchor2.x - i.m_sweep.localCenter.x,
			a = this.m_localAnchor2.y - i.m_sweep.localCenter.y;
			n = t.col1.x * u + t.col2.x * a,
			a = t.col1.y * u + t.col2.y * a,
			u = n,
			this.m_u.x = i.m_sweep.c.x + u - r.m_sweep.c.x - s,
			this.m_u.y = i.m_sweep.c.y + a - r.m_sweep.c.y - o;
			var f = Math.sqrt(this.m_u.x * this.m_u.x + this.m_u.y * this.m_u.y);
			f > d.common.B2Settings.b2_linearSlop ? this.m_u.multiply(1 / f) : this.m_u.setZero();
			var l = s * this.m_u.y - o * this.m_u.x,
			c = u * this.m_u.y - a * this.m_u.x,
			h = r.m_invMass + r.m_invI * l * l + i.m_invMass + i.m_invI * c * c;
			this.m_mass = h != 0 ? 1 / h: 0;
			if (this.m_frequencyHz > 0) {
				var p = f - this.m_length,
				v = 2 * Math.PI * this.m_frequencyHz,
				m = 2 * this.m_mass * this.m_dampingRatio * v,
				g = this.m_mass * v * v;
				this.m_gamma = e.dt * (m + e.dt * g),
				this.m_gamma = this.m_gamma != 0 ? 1 / this.m_gamma: 0,
				this.m_bias = p * e.dt * g * this.m_gamma,
				this.m_mass = h + this.m_gamma,
				this.m_mass = this.m_mass != 0 ? 1 / this.m_mass: 0
			}
			if (e.warmStarting) {
				this.m_impulse *= e.dtRatio;
				var y = this.m_impulse * this.m_u.x,
				b = this.m_impulse * this.m_u.y;
				r.m_linearVelocity.x -= r.m_invMass * y,
				r.m_linearVelocity.y -= r.m_invMass * b,
				r.m_angularVelocity -= r.m_invI * (s * b - o * y),
				i.m_linearVelocity.x += i.m_invMass * y,
				i.m_linearVelocity.y += i.m_invMass * b,
				i.m_angularVelocity += i.m_invI * (u * b - a * y)
			} else this.m_impulse = 0
		},
		setDampingRatio: function (e) {
			this.m_dampingRatio = e
		},
		getDampingRatio: function () {
			return this.m_dampingRatio
		},
		setFrequency: function (e) {
			this.m_frequencyHz = e
		},
		getFrequency: function () {
			return this.m_frequencyHz
		},
		setLength: function (e) {
			this.m_length = e
		},
		getLength: function () {
			return this.m_length
		},
		getReactionTorque: function (e) {
			return 0
		},
		getReactionForce: function (e) {
			return new d.common.math.B2Vec2(e * this.m_impulse * this.m_u.x, e * this.m_impulse * this.m_u.y)
		},
		getAnchorB: function () {
			return this.m_bodyB.getWorldPoint(this.m_localAnchor2)
		},
		getAnchorA: function () {
			return this.m_bodyA.getWorldPoint(this.m_localAnchor1)
		},
		__class__: d.dynamics.joints.B2DistanceJoint
	}),
	d.dynamics.joints.B2JointDef = function () {
		this.type = d.dynamics.joints.B2Joint.e_unknownJoint,
		this.userData = null,
		this.bodyA = null,
		this.bodyB = null,
		this.collideConnected = !1
	},
	d.dynamics.joints.B2JointDef.__name__ = !0,
	d.dynamics.joints.B2JointDef.prototype = {
		__class__: d.dynamics.joints.B2JointDef
	},
	d.dynamics.joints.B2DistanceJointDef = function () {
		d.dynamics.joints.B2JointDef.call(this),
		this.localAnchorA = new d.common.math.B2Vec2,
		this.localAnchorB = new d.common.math.B2Vec2,
		this.type = d.dynamics.joints.B2Joint.e_distanceJoint,
		this.length = 1,
		this.frequencyHz = 0,
		this.dampingRatio = 0
	},
	d.dynamics.joints.B2DistanceJointDef.__name__ = !0,
	d.dynamics.joints.B2DistanceJointDef.__super__ = d.dynamics.joints.B2JointDef,
	d.dynamics.joints.B2DistanceJointDef.prototype = t(d.dynamics.joints.B2JointDef.prototype, {
		initialize: function (e, t, n, r) {
			this.bodyA = e,
			this.bodyB = t,
			this.localAnchorA.setV(this.bodyA.getLocalPoint(n)),
			this.localAnchorB.setV(this.bodyB.getLocalPoint(r));
			var i = r.x - n.x,
			s = r.y - n.y;
			this.length = Math.sqrt(i * i + s * s),
			this.frequencyHz = 0,
			this.dampingRatio = 0
		},
		__class__: d.dynamics.joints.B2DistanceJointDef
	}),
	d.dynamics.joints.B2FrictionJoint = function (e) {
		d.dynamics.joints.B2Joint.call(this, e),
		this.m_localAnchorA = new d.common.math.B2Vec2,
		this.m_localAnchorB = new d.common.math.B2Vec2,
		this.m_linearMass = new d.common.math.B2Mat22,
		this.m_linearImpulse = new d.common.math.B2Vec2,
		this.m_localAnchorA.setV(e.localAnchorA),
		this.m_localAnchorB.setV(e.localAnchorB),
		this.m_linearMass.setZero(),
		this.m_angularMass = 0,
		this.m_linearImpulse.setZero(),
		this.m_angularImpulse = 0,
		this.m_maxForce = e.maxForce,
		this.m_maxTorque = e.maxTorque
	},
	d.dynamics.joints.B2FrictionJoint.__name__ = !0,
	d.dynamics.joints.B2FrictionJoint.__super__ = d.dynamics.joints.B2Joint,
	d.dynamics.joints.B2FrictionJoint.prototype = t(d.dynamics.joints.B2Joint.prototype, {
		solvePositionConstraints: function (e) {
			return ! 0
		},
		solveVelocityConstraints: function (e) {
			var t, n, r = this.m_bodyA,
			i = this.m_bodyB,
			s = r.m_linearVelocity,
			o = r.m_angularVelocity,
			u = i.m_linearVelocity,
			a = i.m_angularVelocity,
			f = r.m_invMass,
			l = i.m_invMass,
			c = r.m_invI,
			h = i.m_invI;
			t = r.m_xf.R;
			var p = this.m_localAnchorA.x - r.m_sweep.localCenter.x,
			v = this.m_localAnchorA.y - r.m_sweep.localCenter.y;
			n = t.col1.x * p + t.col2.x * v,
			v = t.col1.y * p + t.col2.y * v,
			p = n,
			t = i.m_xf.R;
			var m = this.m_localAnchorB.x - i.m_sweep.localCenter.x,
			g = this.m_localAnchorB.y - i.m_sweep.localCenter.y;
			n = t.col1.x * m + t.col2.x * g,
			g = t.col1.y * m + t.col2.y * g,
			m = n;
			var y, b = a - o,
			w = -this.m_angularMass * b,
			E = this.m_angularImpulse;
			y = e.dt * this.m_maxTorque,
			this.m_angularImpulse = d.common.math.B2Math.clamp(this.m_angularImpulse + w, -y, y),
			w = this.m_angularImpulse - E,
			o -= c * w,
			a += h * w;
			var S = u.x - a * g - s.x + o * v,
			x = u.y + a * m - s.y - o * p,
			T = d.common.math.B2Math.mulMV(this.m_linearMass, new d.common.math.B2Vec2( - S, -x)),
			N = this.m_linearImpulse.copy();
			this.m_linearImpulse.add(T),
			y = e.dt * this.m_maxForce,
			this.m_linearImpulse.lengthSquared() > y * y && (this.m_linearImpulse.normalize(), this.m_linearImpulse.multiply(y)),
			T = d.common.math.B2Math.subtractVV(this.m_linearImpulse, N),
			s.x -= f * T.x,
			s.y -= f * T.y,
			o -= c * (p * T.y - v * T.x),
			u.x += l * T.x,
			u.y += l * T.y,
			a += h * (m * T.y - g * T.x),
			r.m_angularVelocity = o,
			i.m_angularVelocity = a
		},
		initVelocityConstraints: function (e) {
			var t, n, r = this.m_bodyA,
			i = this.m_bodyB;
			t = r.m_xf.R;
			var s = this.m_localAnchorA.x - r.m_sweep.localCenter.x,
			o = this.m_localAnchorA.y - r.m_sweep.localCenter.y;
			n = t.col1.x * s + t.col2.x * o,
			o = t.col1.y * s + t.col2.y * o,
			s = n,
			t = i.m_xf.R;
			var u = this.m_localAnchorB.x - i.m_sweep.localCenter.x,
			a = this.m_localAnchorB.y - i.m_sweep.localCenter.y;
			n = t.col1.x * u + t.col2.x * a,
			a = t.col1.y * u + t.col2.y * a,
			u = n;
			var f = r.m_invMass,
			l = i.m_invMass,
			c = r.m_invI,
			h = i.m_invI,
			p = new d.common.math.B2Mat22;
			p.col1.x = f + l,
			p.col2.x = 0,
			p.col1.y = 0,
			p.col2.y = f + l,
			p.col1.x += c * o * o,
			p.col2.x += -c * s * o,
			p.col1.y += -c * s * o,
			p.col2.y += c * s * s,
			p.col1.x += h * a * a,
			p.col2.x += -h * u * a,
			p.col1.y += -h * u * a,
			p.col2.y += h * u * u,
			p.getInverse(this.m_linearMass),
			this.m_angularMass = c + h,
			this.m_angularMass > 0 && (this.m_angularMass = 1 / this.m_angularMass);
			if (e.warmStarting) {
				this.m_linearImpulse.x *= e.dtRatio,
				this.m_linearImpulse.y *= e.dtRatio,
				this.m_angularImpulse *= e.dtRatio;
				var v = this.m_linearImpulse;
				r.m_linearVelocity.x -= f * v.x,
				r.m_linearVelocity.y -= f * v.y,
				r.m_angularVelocity -= c * (s * v.y - o * v.x + this.m_angularImpulse),
				i.m_linearVelocity.x += l * v.x,
				i.m_linearVelocity.y += l * v.y,
				i.m_angularVelocity += h * (u * v.y - a * v.x + this.m_angularImpulse)
			} else this.m_linearImpulse.setZero(),
			this.m_angularImpulse = 0
		},
		getMaxTorque: function () {
			return this.m_maxTorque
		},
		setMaxTorque: function (e) {
			this.m_maxTorque = e
		},
		getMaxForce: function () {
			return this.m_maxForce
		},
		setMaxForce: function (e) {
			this.m_maxForce = e
		},
		getReactionTorque: function (e) {
			return e * this.m_angularImpulse
		},
		getReactionForce: function (e) {
			return new d.common.math.B2Vec2(e * this.m_linearImpulse.x, e * this.m_linearImpulse.y)
		},
		getAnchorB: function () {
			return this.m_bodyB.getWorldPoint(this.m_localAnchorB)
		},
		getAnchorA: function () {
			return this.m_bodyA.getWorldPoint(this.m_localAnchorA)
		},
		__class__: d.dynamics.joints.B2FrictionJoint
	}),
	d.dynamics.joints.B2FrictionJointDef = function () {
		d.dynamics.joints.B2JointDef.call(this),
		this.localAnchorA = new d.common.math.B2Vec2,
		this.localAnchorB = new d.common.math.B2Vec2,
		this.type = d.dynamics.joints.B2Joint.e_frictionJoint,
		this.maxForce = 0,
		this.maxTorque = 0
	},
	d.dynamics.joints.B2FrictionJointDef.__name__ = !0,
	d.dynamics.joints.B2FrictionJointDef.__super__ = d.dynamics.joints.B2JointDef,
	d.dynamics.joints.B2FrictionJointDef.prototype = t(d.dynamics.joints.B2JointDef.prototype, {
		initialize: function (e, t, n) {
			this.bodyA = e,
			this.bodyB = t,
			this.localAnchorA.setV(this.bodyA.getLocalPoint(n)),
			this.localAnchorB.setV(this.bodyB.getLocalPoint(n))
		},
		__class__: d.dynamics.joints.B2FrictionJointDef
	}),
	d.dynamics.joints.B2GearJoint = function (e) {
		d.dynamics.joints.B2Joint.call(this, e),
		this.m_groundAnchor1 = new d.common.math.B2Vec2,
		this.m_groundAnchor2 = new d.common.math.B2Vec2,
		this.m_localAnchor1 = new d.common.math.B2Vec2,
		this.m_localAnchor2 = new d.common.math.B2Vec2,
		this.m_J = new d.dynamics.joints.B2Jacobian;
		var t = e.joint1.m_type,
		n = e.joint2.m_type;
		this.m_revolute1 = null,
		this.m_prismatic1 = null,
		this.m_revolute2 = null,
		this.m_prismatic2 = null;
		var r, i;
		this.m_ground1 = e.joint1.getBodyA(),
		this.m_bodyA = e.joint1.getBodyB(),
		t == d.dynamics.joints.B2Joint.e_revoluteJoint ? (this.m_revolute1 = y.Boot.__cast(e.joint1, d.dynamics.joints.B2RevoluteJoint), this.m_groundAnchor1.setV(this.m_revolute1.m_localAnchor1), this.m_localAnchor1.setV(this.m_revolute1.m_localAnchor2), r = this.m_revolute1.getJointAngle()) : (this.m_prismatic1 = y.Boot.__cast(e.joint1, d.dynamics.joints.B2PrismaticJoint), this.m_groundAnchor1.setV(this.m_prismatic1.m_localAnchor1), this.m_localAnchor1.setV(this.m_prismatic1.m_localAnchor2), r = this.m_prismatic1.getJointTranslation()),
		this.m_ground2 = e.joint2.getBodyA(),
		this.m_bodyB = e.joint2.getBodyB(),
		n == d.dynamics.joints.B2Joint.e_revoluteJoint ? (this.m_revolute2 = y.Boot.__cast(e.joint2, d.dynamics.joints.B2RevoluteJoint), this.m_groundAnchor2.setV(this.m_revolute2.m_localAnchor1), this.m_localAnchor2.setV(this.m_revolute2.m_localAnchor2), i = this.m_revolute2.getJointAngle()) : (this.m_prismatic2 = y.Boot.__cast(e.joint2, d.dynamics.joints.B2PrismaticJoint), this.m_groundAnchor2.setV(this.m_prismatic2.m_localAnchor1), this.m_localAnchor2.setV(this.m_prismatic2.m_localAnchor2), i = this.m_prismatic2.getJointTranslation()),
		this.m_ratio = e.ratio,
		this.m_constant = r + this.m_ratio * i,
		this.m_impulse = 0
	},
	d.dynamics.joints.B2GearJoint.__name__ = !0,
	d.dynamics.joints.B2GearJoint.__super__ = d.dynamics.joints.B2Joint,
	d.dynamics.joints.B2GearJoint.prototype = t(d.dynamics.joints.B2Joint.prototype, {
		solvePositionConstraints: function (e) {
			var t = 0,
			n = this.m_bodyA,
			r = this.m_bodyB,
			i, s;
			this.m_revolute1 != null ? i = this.m_revolute1.getJointAngle() : i = this.m_prismatic1.getJointTranslation(),
			this.m_revolute2 != null ? s = this.m_revolute2.getJointAngle() : s = this.m_prismatic2.getJointTranslation();
			var o = this.m_constant - (i + this.m_ratio * s),
			u = -this.m_mass * o;
			return n.m_sweep.c.x += n.m_invMass * u * this.m_J.linearA.x,
			n.m_sweep.c.y += n.m_invMass * u * this.m_J.linearA.y,
			n.m_sweep.a += n.m_invI * u * this.m_J.angularA,
			r.m_sweep.c.x += r.m_invMass * u * this.m_J.linearB.x,
			r.m_sweep.c.y += r.m_invMass * u * this.m_J.linearB.y,
			r.m_sweep.a += r.m_invI * u * this.m_J.angularB,
			n.synchronizeTransform(),
			r.synchronizeTransform(),
			t < d.common.B2Settings.b2_linearSlop
		},
		solveVelocityConstraints: function (e) {
			var t = this.m_bodyA,
			n = this.m_bodyB,
			r = this.m_J.compute(t.m_linearVelocity, t.m_angularVelocity, n.m_linearVelocity, n.m_angularVelocity),
			i = -this.m_mass * r;
			this.m_impulse += i,
			t.m_linearVelocity.x += t.m_invMass * i * this.m_J.linearA.x,
			t.m_linearVelocity.y += t.m_invMass * i * this.m_J.linearA.y,
			t.m_angularVelocity += t.m_invI * i * this.m_J.angularA,
			n.m_linearVelocity.x += n.m_invMass * i * this.m_J.linearB.x,
			n.m_linearVelocity.y += n.m_invMass * i * this.m_J.linearB.y,
			n.m_angularVelocity += n.m_invI * i * this.m_J.angularB
		},
		initVelocityConstraints: function (e) {
			var t = this.m_ground1,
			n = this.m_ground2,
			r = this.m_bodyA,
			i = this.m_bodyB,
			s, o, u, a, f, l, c, h, p = 0;
			this.m_J.setZero(),
			this.m_revolute1 != null ? (this.m_J.angularA = -1, p += r.m_invI) : (f = t.m_xf.R, l = this.m_prismatic1.m_localXAxis1, s = f.col1.x * l.x + f.col2.x * l.y, o = f.col1.y * l.x + f.col2.y * l.y, f = r.m_xf.R, u = this.m_localAnchor1.x - r.m_sweep.localCenter.x, a = this.m_localAnchor1.y - r.m_sweep.localCenter.y, h = f.col1.x * u + f.col2.x * a, a = f.col1.y * u + f.col2.y * a, u = h, c = u * o - a * s, this.m_J.linearA.set( - s, -o), this.m_J.angularA = -c, p += r.m_invMass + r.m_invI * c * c),
			this.m_revolute2 != null ? (this.m_J.angularB = -this.m_ratio, p += this.m_ratio * this.m_ratio * i.m_invI) : (f = n.m_xf.R, l = this.m_prismatic2.m_localXAxis1, s = f.col1.x * l.x + f.col2.x * l.y, o = f.col1.y * l.x + f.col2.y * l.y, f = i.m_xf.R, u = this.m_localAnchor2.x - i.m_sweep.localCenter.x, a = this.m_localAnchor2.y - i.m_sweep.localCenter.y, h = f.col1.x * u + f.col2.x * a, a = f.col1.y * u + f.col2.y * a, u = h, c = u * o - a * s, this.m_J.linearB.set( - this.m_ratio * s, -this.m_ratio * o), this.m_J.angularB = -this.m_ratio * c, p += this.m_ratio * this.m_ratio * (i.m_invMass + i.m_invI * c * c)),
			this.m_mass = p > 0 ? 1 / p: 0,
			e.warmStarting ? (r.m_linearVelocity.x += r.m_invMass * this.m_impulse * this.m_J.linearA.x, r.m_linearVelocity.y += r.m_invMass * this.m_impulse * this.m_J.linearA.y, r.m_angularVelocity += r.m_invI * this.m_impulse * this.m_J.angularA, i.m_linearVelocity.x += i.m_invMass * this.m_impulse * this.m_J.linearB.x, i.m_linearVelocity.y += i.m_invMass * this.m_impulse * this.m_J.linearB.y, i.m_angularVelocity += i.m_invI * this.m_impulse * this.m_J.angularB) : this.m_impulse = 0
		},
		setRatio: function (e) {
			this.m_ratio = e
		},
		getRatio: function () {
			return this.m_ratio
		},
		getReactionTorque: function (e) {
			var t = this.m_bodyB.m_xf.R,
			n = this.m_localAnchor1.x - this.m_bodyB.m_sweep.localCenter.x,
			r = this.m_localAnchor1.y - this.m_bodyB.m_sweep.localCenter.y,
			i = t.col1.x * n + t.col2.x * r;
			r = t.col1.y * n + t.col2.y * r,
			n = i;
			var s = this.m_impulse * this.m_J.linearB.x,
			o = this.m_impulse * this.m_J.linearB.y;
			return e * (this.m_impulse * this.m_J.angularB - n * o + r * s)
		},
		getReactionForce: function (e) {
			return new d.common.math.B2Vec2(e * this.m_impulse * this.m_J.linearB.x, e * this.m_impulse * this.m_J.linearB.y)
		},
		getAnchorB: function () {
			return this.m_bodyB.getWorldPoint(this.m_localAnchor2)
		},
		getAnchorA: function () {
			return this.m_bodyA.getWorldPoint(this.m_localAnchor1)
		},
		__class__: d.dynamics.joints.B2GearJoint
	}),
	d.dynamics.joints.B2GearJointDef = function () {
		d.dynamics.joints.B2JointDef.call(this),
		this.type = d.dynamics.joints.B2Joint.e_gearJoint,
		this.joint1 = null,
		this.joint2 = null,
		this.ratio = 1
	},
	d.dynamics.joints.B2GearJointDef.__name__ = !0,
	d.dynamics.joints.B2GearJointDef.__super__ = d.dynamics.joints.B2JointDef,
	d.dynamics.joints.B2GearJointDef.prototype = t(d.dynamics.joints.B2JointDef.prototype, {
		__class__: d.dynamics.joints.B2GearJointDef
	}),
	d.dynamics.joints.B2Jacobian = function () {
		this.linearA = new d.common.math.B2Vec2,
		this.linearB = new d.common.math.B2Vec2
	},
	d.dynamics.joints.B2Jacobian.__name__ = !0,
	d.dynamics.joints.B2Jacobian.prototype = {
		compute: function (e, t, n, r) {
			return this.linearA.x * e.x + this.linearA.y * e.y + this.angularA * t + (this.linearB.x * n.x + this.linearB.y * n.y) + this.angularB * r
		},
		set: function (e, t, n, r) {
			this.linearA.setV(e),
			this.angularA = t,
			this.linearB.setV(n),
			this.angularB = r
		},
		setZero: function () {
			this.linearA.setZero(),
			this.angularA = 0,
			this.linearB.setZero(),
			this.angularB = 0
		},
		__class__: d.dynamics.joints.B2Jacobian
	},
	d.dynamics.joints.B2JointEdge = function () {},
	d.dynamics.joints.B2JointEdge.__name__ = !0,
	d.dynamics.joints.B2JointEdge.prototype = {
		__class__: d.dynamics.joints.B2JointEdge
	},
	d.dynamics.joints.B2LineJoint = function (e) {
		d.dynamics.joints.B2Joint.call(this, e),
		this.m_localAnchor1 = new d.common.math.B2Vec2,
		this.m_localAnchor2 = new d.common.math.B2Vec2,
		this.m_localXAxis1 = new d.common.math.B2Vec2,
		this.m_localYAxis1 = new d.common.math.B2Vec2,
		this.m_axis = new d.common.math.B2Vec2,
		this.m_perp = new d.common.math.B2Vec2,
		this.m_K = new d.common.math.B2Mat22,
		this.m_impulse = new d.common.math.B2Vec2;
		var t, n, r;
		this.m_localAnchor1.setV(e.localAnchorA),
		this.m_localAnchor2.setV(e.localAnchorB),
		this.m_localXAxis1.setV(e.localAxisA),
		this.m_localYAxis1.x = -this.m_localXAxis1.y,
		this.m_localYAxis1.y = this.m_localXAxis1.x,
		this.m_impulse.setZero(),
		this.m_motorMass = 0,
		this.m_motorImpulse = 0,
		this.m_lowerTranslation = e.lowerTranslation,
		this.m_upperTranslation = e.upperTranslation,
		this.m_maxMotorForce = e.maxMotorForce,
		this.m_motorSpeed = e.motorSpeed,
		this.m_enableLimit = e.enableLimit,
		this.m_enableMotor = e.enableMotor,
		this.m_limitState = d.dynamics.joints.B2Joint.e_inactiveLimit,
		this.m_axis.setZero(),
		this.m_perp.setZero()
	},
	d.dynamics.joints.B2LineJoint.__name__ = !0,
	d.dynamics.joints.B2LineJoint.__super__ = d.dynamics.joints.B2Joint,
	d.dynamics.joints.B2LineJoint.prototype = t(d.dynamics.joints.B2Joint.prototype, {
		solvePositionConstraints: function (e) {
			var t, n, r = this.m_bodyA,
			i = this.m_bodyB,
			s = r.m_sweep.c,
			o = r.m_sweep.a,
			u = i.m_sweep.c,
			a = i.m_sweep.a,
			f, l, c, h, p, v, m = 0,
			g = 0,
			y = !1,
			b = 0,
			w = d.common.math.B2Mat22.fromAngle(o),
			E = d.common.math.B2Mat22.fromAngle(a);
			f = w;
			var S = this.m_localAnchor1.x - this.m_localCenterA.x,
			x = this.m_localAnchor1.y - this.m_localCenterA.y;
			l = f.col1.x * S + f.col2.x * x,
			x = f.col1.y * S + f.col2.y * x,
			S = l,
			f = E;
			var T = this.m_localAnchor2.x - this.m_localCenterB.x,
			N = this.m_localAnchor2.y - this.m_localCenterB.y;
			l = f.col1.x * T + f.col2.x * N,
			N = f.col1.y * T + f.col2.y * N,
			T = l;
			var C = u.x + T - s.x - S,
			k = u.y + N - s.y - x;
			if (this.m_enableLimit) {
				this.m_axis = d.common.math.B2Math.mulMV(w, this.m_localXAxis1),
				this.m_a1 = (C + S) * this.m_axis.y - (k + x) * this.m_axis.x,
				this.m_a2 = T * this.m_axis.y - N * this.m_axis.x;
				var L = this.m_axis.x * C + this.m_axis.y * k;
				d.common.math.B2Math.abs(this.m_upperTranslation - this.m_lowerTranslation) < 2 * d.common.B2Settings.b2_linearSlop ? (b = d.common.math.B2Math.clamp(L, -d.common.B2Settings.b2_maxLinearCorrection, d.common.B2Settings.b2_maxLinearCorrection), m = d.common.math.B2Math.abs(L), y = !0) : L <= this.m_lowerTranslation ? (b = d.common.math.B2Math.clamp(L - this.m_lowerTranslation + d.common.B2Settings.b2_linearSlop, -d.common.B2Settings.b2_maxLinearCorrection, 0), m = this.m_lowerTranslation - L, y = !0) : L >= this.m_upperTranslation && (b = d.common.math.B2Math.clamp(L - this.m_upperTranslation + d.common.B2Settings.b2_linearSlop, 0, d.common.B2Settings.b2_maxLinearCorrection), m = L - this.m_upperTranslation, y = !0)
			}
			this.m_perp = d.common.math.B2Math.mulMV(w, this.m_localYAxis1),
			this.m_s1 = (C + S) * this.m_perp.y - (k + x) * this.m_perp.x,
			this.m_s2 = T * this.m_perp.y - N * this.m_perp.x;
			var A = new d.common.math.B2Vec2,
			O = this.m_perp.x * C + this.m_perp.y * k;
			m = d.common.math.B2Math.max(m, d.common.math.B2Math.abs(O)),
			g = 0;
			if (y) c = this.m_invMassA,
			h = this.m_invMassB,
			p = this.m_invIA,
			v = this.m_invIB,
			this.m_K.col1.x = c + h + p * this.m_s1 * this.m_s1 + v * this.m_s2 * this.m_s2,
			this.m_K.col1.y = p * this.m_s1 * this.m_a1 + v * this.m_s2 * this.m_a2,
			this.m_K.col2.x = this.m_K.col1.y,
			this.m_K.col2.y = c + h + p * this.m_a1 * this.m_a1 + v * this.m_a2 * this.m_a2,
			this.m_K.solve(A, -O, -b);
			else {
				c = this.m_invMassA,
				h = this.m_invMassB,
				p = this.m_invIA,
				v = this.m_invIB;
				var M = c + h + p * this.m_s1 * this.m_s1 + v * this.m_s2 * this.m_s2,
				_;
				M != 0 ? _ = -O / M: _ = 0,
				A.x = _,
				A.y = 0
			}
			var D = A.x * this.m_perp.x + A.y * this.m_axis.x,
			P = A.x * this.m_perp.y + A.y * this.m_axis.y,
			H = A.x * this.m_s1 + A.y * this.m_a1,
			B = A.x * this.m_s2 + A.y * this.m_a2;
			return s.x -= this.m_invMassA * D,
			s.y -= this.m_invMassA * P,
			o -= this.m_invIA * H,
			u.x += this.m_invMassB * D,
			u.y += this.m_invMassB * P,
			a += this.m_invIB * B,
			r.m_sweep.a = o,
			i.m_sweep.a = a,
			r.synchronizeTransform(),
			i.synchronizeTransform(),
			m <= d.common.B2Settings.b2_linearSlop && g <= d.common.B2Settings.b2_angularSlop
		},
		solveVelocityConstraints: function (e) {
			var t = this.m_bodyA,
			n = this.m_bodyB,
			r = t.m_linearVelocity,
			i = t.m_angularVelocity,
			s = n.m_linearVelocity,
			o = n.m_angularVelocity,
			u, a, f, l;
			if (this.m_enableMotor && this.m_limitState != d.dynamics.joints.B2Joint.e_equalLimits) {
				var c = this.m_axis.x * (s.x - r.x) + this.m_axis.y * (s.y - r.y) + this.m_a2 * o - this.m_a1 * i,
				h = this.m_motorMass * (this.m_motorSpeed - c),
				p = this.m_motorImpulse,
				v = e.dt * this.m_maxMotorForce;
				this.m_motorImpulse = d.common.math.B2Math.clamp(this.m_motorImpulse + h, -v, v),
				h = this.m_motorImpulse - p,
				u = h * this.m_axis.x,
				a = h * this.m_axis.y,
				f = h * this.m_a1,
				l = h * this.m_a2,
				r.x -= this.m_invMassA * u,
				r.y -= this.m_invMassA * a,
				i -= this.m_invIA * f,
				s.x += this.m_invMassB * u,
				s.y += this.m_invMassB * a,
				o += this.m_invIB * l
			}
			var m = this.m_perp.x * (s.x - r.x) + this.m_perp.y * (s.y - r.y) + this.m_s2 * o - this.m_s1 * i;
			if (this.m_enableLimit && this.m_limitState != d.dynamics.joints.B2Joint.e_inactiveLimit) {
				var g = this.m_axis.x * (s.x - r.x) + this.m_axis.y * (s.y - r.y) + this.m_a2 * o - this.m_a1 * i,
				y = this.m_impulse.copy(),
				b = this.m_K.solve(new d.common.math.B2Vec2, -m, -g);
				this.m_impulse.add(b),
				this.m_limitState == d.dynamics.joints.B2Joint.e_atLowerLimit ? this.m_impulse.y = d.common.math.B2Math.max(this.m_impulse.y, 0) : this.m_limitState == d.dynamics.joints.B2Joint.e_atUpperLimit && (this.m_impulse.y = d.common.math.B2Math.min(this.m_impulse.y, 0));
				var w = -m - (this.m_impulse.y - y.y) * this.m_K.col2.x,
				E;
				this.m_K.col1.x != 0 ? E = w / this.m_K.col1.x + y.x: E = y.x,
				this.m_impulse.x = E,
				b.x = this.m_impulse.x - y.x,
				b.y = this.m_impulse.y - y.y,
				u = b.x * this.m_perp.x + b.y * this.m_axis.x,
				a = b.x * this.m_perp.y + b.y * this.m_axis.y,
				f = b.x * this.m_s1 + b.y * this.m_a1,
				l = b.x * this.m_s2 + b.y * this.m_a2,
				r.x -= this.m_invMassA * u,
				r.y -= this.m_invMassA * a,
				i -= this.m_invIA * f,
				s.x += this.m_invMassB * u,
				s.y += this.m_invMassB * a,
				o += this.m_invIB * l
			} else {
				var S;
				this.m_K.col1.x != 0 ? S = -m / this.m_K.col1.x: S = 0,
				this.m_impulse.x += S,
				u = S * this.m_perp.x,
				a = S * this.m_perp.y,
				f = S * this.m_s1,
				l = S * this.m_s2,
				r.x -= this.m_invMassA * u,
				r.y -= this.m_invMassA * a,
				i -= this.m_invIA * f,
				s.x += this.m_invMassB * u,
				s.y += this.m_invMassB * a,
				o += this.m_invIB * l
			}
			t.m_linearVelocity.setV(r),
			t.m_angularVelocity = i,
			n.m_linearVelocity.setV(s),
			n.m_angularVelocity = o
		},
		initVelocityConstraints: function (e) {
			var t = this.m_bodyA,
			n = this.m_bodyB,
			r, i;
			this.m_localCenterA.setV(t.getLocalCenter()),
			this.m_localCenterB.setV(n.getLocalCenter());
			var s = t.getTransform(),
			o = n.getTransform();
			r = t.m_xf.R;
			var u = this.m_localAnchor1.x - this.m_localCenterA.x,
			a = this.m_localAnchor1.y - this.m_localCenterA.y;
			i = r.col1.x * u + r.col2.x * a,
			a = r.col1.y * u + r.col2.y * a,
			u = i,
			r = n.m_xf.R;
			var f = this.m_localAnchor2.x - this.m_localCenterB.x,
			l = this.m_localAnchor2.y - this.m_localCenterB.y;
			i = r.col1.x * f + r.col2.x * l,
			l = r.col1.y * f + r.col2.y * l,
			f = i;
			var c = n.m_sweep.c.x + f - t.m_sweep.c.x - u,
			h = n.m_sweep.c.y + l - t.m_sweep.c.y - a;
			this.m_invMassA = t.m_invMass,
			this.m_invMassB = n.m_invMass,
			this.m_invIA = t.m_invI,
			this.m_invIB = n.m_invI,
			this.m_axis.setV(d.common.math.B2Math.mulMV(s.R, this.m_localXAxis1)),
			this.m_a1 = (c + u) * this.m_axis.y - (h + a) * this.m_axis.x,
			this.m_a2 = f * this.m_axis.y - l * this.m_axis.x,
			this.m_motorMass = this.m_invMassA + this.m_invMassB + this.m_invIA * this.m_a1 * this.m_a1 + this.m_invIB * this.m_a2 * this.m_a2,
			this.m_motorMass = this.m_motorMass > 2.2250738585072014e-308 ? 1 / this.m_motorMass: 0,
			this.m_perp.setV(d.common.math.B2Math.mulMV(s.R, this.m_localYAxis1)),
			this.m_s1 = (c + u) * this.m_perp.y - (h + a) * this.m_perp.x,
			this.m_s2 = f * this.m_perp.y - l * this.m_perp.x;
			var p = this.m_invMassA,
			v = this.m_invMassB,
			m = this.m_invIA,
			g = this.m_invIB;
			this.m_K.col1.x = p + v + m * this.m_s1 * this.m_s1 + g * this.m_s2 * this.m_s2,
			this.m_K.col1.y = m * this.m_s1 * this.m_a1 + g * this.m_s2 * this.m_a2,
			this.m_K.col2.x = this.m_K.col1.y,
			this.m_K.col2.y = p + v + m * this.m_a1 * this.m_a1 + g * this.m_a2 * this.m_a2;
			if (this.m_enableLimit) {
				var y = this.m_axis.x * c + this.m_axis.y * h;
				d.common.math.B2Math.abs(this.m_upperTranslation - this.m_lowerTranslation) < 2 * d.common.B2Settings.b2_linearSlop ? this.m_limitState = d.dynamics.joints.B2Joint.e_equalLimits: y <= this.m_lowerTranslation ? this.m_limitState != d.dynamics.joints.B2Joint.e_atLowerLimit && (this.m_limitState = d.dynamics.joints.B2Joint.e_atLowerLimit, this.m_impulse.y = 0) : y >= this.m_upperTranslation ? this.m_limitState != d.dynamics.joints.B2Joint.e_atUpperLimit && (this.m_limitState = d.dynamics.joints.B2Joint.e_atUpperLimit, this.m_impulse.y = 0) : (this.m_limitState = d.dynamics.joints.B2Joint.e_inactiveLimit, this.m_impulse.y = 0)
			} else this.m_limitState = d.dynamics.joints.B2Joint.e_inactiveLimit;
			this.m_enableMotor == 0 && (this.m_motorImpulse = 0);
			if (e.warmStarting) {
				this.m_impulse.x *= e.dtRatio,
				this.m_impulse.y *= e.dtRatio,
				this.m_motorImpulse *= e.dtRatio;
				var b = this.m_impulse.x * this.m_perp.x + (this.m_motorImpulse + this.m_impulse.y) * this.m_axis.x,
				w = this.m_impulse.x * this.m_perp.y + (this.m_motorImpulse + this.m_impulse.y) * this.m_axis.y,
				E = this.m_impulse.x * this.m_s1 + (this.m_motorImpulse + this.m_impulse.y) * this.m_a1,
				S = this.m_impulse.x * this.m_s2 + (this.m_motorImpulse + this.m_impulse.y) * this.m_a2;
				t.m_linearVelocity.x -= this.m_invMassA * b,
				t.m_linearVelocity.y -= this.m_invMassA * w,
				t.m_angularVelocity -= this.m_invIA * E,
				n.m_linearVelocity.x += this.m_invMassB * b,
				n.m_linearVelocity.y += this.m_invMassB * w,
				n.m_angularVelocity += this.m_invIB * S
			} else this.m_impulse.setZero(),
			this.m_motorImpulse = 0
		},
		getMotorForce: function () {
			return this.m_motorImpulse
		},
		getMaxMotorForce: function () {
			return this.m_maxMotorForce
		},
		setMaxMotorForce: function (e) {
			this.m_bodyA.setAwake(!0),
			this.m_bodyB.setAwake(!0),
			this.m_maxMotorForce = e
		},
		getMotorSpeed: function () {
			return this.m_motorSpeed
		},
		setMotorSpeed: function (e) {
			this.m_bodyA.setAwake(!0),
			this.m_bodyB.setAwake(!0),
			this.m_motorSpeed = e
		},
		enableMotor: function (e) {
			this.m_bodyA.setAwake(!0),
			this.m_bodyB.setAwake(!0),
			this.m_enableMotor = e
		},
		isMotorEnabled: function () {
			return this.m_enableMotor
		},
		setLimits: function (e, t) {
			this.m_bodyA.setAwake(!0),
			this.m_bodyB.setAwake(!0),
			this.m_lowerTranslation = e,
			this.m_upperTranslation = t
		},
		getUpperLimit: function () {
			return this.m_upperTranslation
		},
		getLowerLimit: function () {
			return this.m_lowerTranslation
		},
		enableLimit: function (e) {
			this.m_bodyA.setAwake(!0),
			this.m_bodyB.setAwake(!0),
			this.m_enableLimit = e
		},
		isLimitEnabled: function () {
			return this.m_enableLimit
		},
		getJointSpeed: function () {
			var e = this.m_bodyA,
			t = this.m_bodyB,
			n;
			n = e.m_xf.R;
			var r = this.m_localAnchor1.x - e.m_sweep.localCenter.x,
			i = this.m_localAnchor1.y - e.m_sweep.localCenter.y,
			s = n.col1.x * r + n.col2.x * i;
			i = n.col1.y * r + n.col2.y * i,
			r = s,
			n = t.m_xf.R;
			var o = this.m_localAnchor2.x - t.m_sweep.localCenter.x,
			u = this.m_localAnchor2.y - t.m_sweep.localCenter.y;
			s = n.col1.x * o + n.col2.x * u,
			u = n.col1.y * o + n.col2.y * u,
			o = s;
			var a = e.m_sweep.c.x + r,
			f = e.m_sweep.c.y + i,
			l = t.m_sweep.c.x + o,
			c = t.m_sweep.c.y + u,
			h = l - a,
			p = c - f,
			d = e.getWorldVector(this.m_localXAxis1),
			v = e.m_linearVelocity,
			m = t.m_linearVelocity,
			g = e.m_angularVelocity,
			y = t.m_angularVelocity,
			b = h * -g * d.y + p * g * d.x + (d.x * (m.x + -y * u - v.x - -g * i) + d.y * (m.y + y * o - v.y - g * r));
			return b
		},
		getJointTranslation: function () {
			var e = this.m_bodyA,
			t = this.m_bodyB,
			n, r = e.getWorldPoint(this.m_localAnchor1),
			i = t.getWorldPoint(this.m_localAnchor2),
			s = i.x - r.x,
			o = i.y - r.y,
			u = e.getWorldVector(this.m_localXAxis1),
			a = u.x * s + u.y * o;
			return a
		},
		getReactionTorque: function (e) {
			return e * this.m_impulse.y
		},
		getReactionForce: function (e) {
			return new d.common.math.B2Vec2(e * (this.m_impulse.x * this.m_perp.x + (this.m_motorImpulse + this.m_impulse.y) * this.m_axis.x), e * (this.m_impulse.x * this.m_perp.y + (this.m_motorImpulse + this.m_impulse.y) * this.m_axis.y))
		},
		getAnchorB: function () {
			return this.m_bodyB.getWorldPoint(this.m_localAnchor2)
		},
		getAnchorA: function () {
			return this.m_bodyA.getWorldPoint(this.m_localAnchor1)
		},
		__class__: d.dynamics.joints.B2LineJoint
	}),
	d.dynamics.joints.B2LineJointDef = function () {
		d.dynamics.joints.B2JointDef.call(this)
	},
	d.dynamics.joints.B2LineJointDef.__name__ = !0,
	d.dynamics.joints.B2LineJointDef.__super__ = d.dynamics.joints.B2JointDef,
	d.dynamics.joints.B2LineJointDef.prototype = t(d.dynamics.joints.B2JointDef.prototype, {
		initialize: function (e, t, n, r) {
			this.bodyA = e,
			this.bodyB = t,
			this.localAnchorA = this.bodyA.getLocalPoint(n),
			this.localAnchorB = this.bodyB.getLocalPoint(n),
			this.localAxisA = this.bodyA.getLocalVector(r)
		},
		b2LineJointDef: function () {
			this.localAnchorA = new d.common.math.B2Vec2,
			this.localAnchorB = new d.common.math.B2Vec2,
			this.localAxisA = new d.common.math.B2Vec2,
			this.type = d.dynamics.joints.B2Joint.e_lineJoint,
			this.localAxisA.set(1, 0),
			this.enableLimit = !1,
			this.lowerTranslation = 0,
			this.upperTranslation = 0,
			this.enableMotor = !1,
			this.maxMotorForce = 0,
			this.motorSpeed = 0
		},
		__class__: d.dynamics.joints.B2LineJointDef
	}),
	d.dynamics.joints.B2MouseJoint = function (e) {
		d.dynamics.joints.B2Joint.call(this, e),
		this.K = new d.common.math.B2Mat22,
		this.K1 = new d.common.math.B2Mat22,
		this.K2 = new d.common.math.B2Mat22,
		this.m_localAnchor = new d.common.math.B2Vec2,
		this.m_target = new d.common.math.B2Vec2,
		this.m_impulse = new d.common.math.B2Vec2,
		this.m_mass = new d.common.math.B2Mat22,
		this.m_C = new d.common.math.B2Vec2,
		this.m_target.setV(e.target);
		var t = this.m_target.x - this.m_bodyB.m_xf.position.x,
		n = this.m_target.y - this.m_bodyB.m_xf.position.y,
		r = this.m_bodyB.m_xf.R;
		this.m_localAnchor.x = t * r.col1.x + n * r.col1.y,
		this.m_localAnchor.y = t * r.col2.x + n * r.col2.y,
		this.m_maxForce = e.maxForce,
		this.m_impulse.setZero(),
		this.m_frequencyHz = e.frequencyHz,
		this.m_dampingRatio = e.dampingRatio,
		this.m_beta = 0,
		this.m_gamma = 0
	},
	d.dynamics.joints.B2MouseJoint.__name__ = !0,
	d.dynamics.joints.B2MouseJoint.__super__ = d.dynamics.joints.B2Joint,
	d.dynamics.joints.B2MouseJoint.prototype = t(d.dynamics.joints.B2Joint.prototype, {
		solvePositionConstraints: function (e) {
			return ! 0
		},
		solveVelocityConstraints: function (e) {
			var t = this.m_bodyB,
			n, r, i;
			n = t.m_xf.R;
			var s = this.m_localAnchor.x - t.m_sweep.localCenter.x,
			o = this.m_localAnchor.y - t.m_sweep.localCenter.y;
			r = n.col1.x * s + n.col2.x * o,
			o = n.col1.y * s + n.col2.y * o,
			s = r;
			var u = t.m_linearVelocity.x + -t.m_angularVelocity * o,
			a = t.m_linearVelocity.y + t.m_angularVelocity * s;
			n = this.m_mass,
			r = u + this.m_beta * this.m_C.x + this.m_gamma * this.m_impulse.x,
			i = a + this.m_beta * this.m_C.y + this.m_gamma * this.m_impulse.y;
			var f = -(n.col1.x * r + n.col2.x * i),
			l = -(n.col1.y * r + n.col2.y * i),
			c = this.m_impulse.x,
			h = this.m_impulse.y;
			this.m_impulse.x += f,
			this.m_impulse.y += l;
			var p = e.dt * this.m_maxForce;
			this.m_impulse.lengthSquared() > p * p && this.m_impulse.multiply(p / this.m_impulse.length()),
			f = this.m_impulse.x - c,
			l = this.m_impulse.y - h,
			t.m_linearVelocity.x += t.m_invMass * f,
			t.m_linearVelocity.y += t.m_invMass * l,
			t.m_angularVelocity += t.m_invI * (s * l - o * f)
		},
		initVelocityConstraints: function (e) {
			var t = this.m_bodyB,
			n = t.getMass(),
			r = 2 * Math.PI * this.m_frequencyHz,
			i = 2 * n * this.m_dampingRatio * r,
			s = n * r * r;
			this.m_gamma = e.dt * (i + e.dt * s),
			this.m_gamma = this.m_gamma != 0 ? 1 / this.m_gamma: 0,
			this.m_beta = e.dt * s * this.m_gamma;
			var o;
			o = t.m_xf.R;
			var u = this.m_localAnchor.x - t.m_sweep.localCenter.x,
			a = this.m_localAnchor.y - t.m_sweep.localCenter.y,
			f = o.col1.x * u + o.col2.x * a;
			a = o.col1.y * u + o.col2.y * a,
			u = f;
			var l = t.m_invMass,
			c = t.m_invI;
			this.K1.col1.x = l,
			this.K1.col2.x = 0,
			this.K1.col1.y = 0,
			this.K1.col2.y = l,
			this.K2.col1.x = c * a * a,
			this.K2.col2.x = -c * u * a,
			this.K2.col1.y = -c * u * a,
			this.K2.col2.y = c * u * u,
			this.K.setM(this.K1),
			this.K.addM(this.K2),
			this.K.col1.x += this.m_gamma,
			this.K.col2.y += this.m_gamma,
			this.K.getInverse(this.m_mass),
			this.m_C.x = t.m_sweep.c.x + u - this.m_target.x,
			this.m_C.y = t.m_sweep.c.y + a - this.m_target.y,
			t.m_angularVelocity *= .98,
			this.m_impulse.x *= e.dtRatio,
			this.m_impulse.y *= e.dtRatio,
			t.m_linearVelocity.x += l * this.m_impulse.x,
			t.m_linearVelocity.y += l * this.m_impulse.y,
			t.m_angularVelocity += c * (u * this.m_impulse.y - a * this.m_impulse.x)
		},
		setDampingRatio: function (e) {
			this.m_dampingRatio = e
		},
		getDampingRatio: function () {
			return this.m_dampingRatio
		},
		setFrequency: function (e) {
			this.m_frequencyHz = e
		},
		getFrequency: function () {
			return this.m_frequencyHz
		},
		setMaxForce: function (e) {
			this.m_maxForce = e
		},
		getMaxForce: function () {
			return this.m_maxForce
		},
		setTarget: function (e) {
			this.m_bodyB.isAwake() == 0 && this.m_bodyB.setAwake(!0),
			this.m_target = e
		},
		getTarget: function () {
			return this.m_target
		},
		getReactionTorque: function (e) {
			return 0
		},
		getReactionForce: function (e) {
			return new d.common.math.B2Vec2(e * this.m_impulse.x, e * this.m_impulse.y)
		},
		getAnchorB: function () {
			return this.m_bodyB.getWorldPoint(this.m_localAnchor)
		},
		getAnchorA: function () {
			return this.m_target
		},
		__class__: d.dynamics.joints.B2MouseJoint
	}),
	d.dynamics.joints.B2MouseJointDef = function () {
		d.dynamics.joints.B2JointDef.call(this),
		this.target = new d.common.math.B2Vec2,
		this.type = d.dynamics.joints.B2Joint.e_mouseJoint,
		this.maxForce = 0,
		this.frequencyHz = 5,
		this.dampingRatio = .7
	},
	d.dynamics.joints.B2MouseJointDef.__name__ = !0,
	d.dynamics.joints.B2MouseJointDef.__super__ = d.dynamics.joints.B2JointDef,
	d.dynamics.joints.B2MouseJointDef.prototype = t(d.dynamics.joints.B2JointDef.prototype, {
		__class__: d.dynamics.joints.B2MouseJointDef
	}),
	d.dynamics.joints.B2PrismaticJoint = function (e) {
		d.dynamics.joints.B2Joint.call(this, e),
		this.m_localAnchor1 = new d.common.math.B2Vec2,
		this.m_localAnchor2 = new d.common.math.B2Vec2,
		this.m_localXAxis1 = new d.common.math.B2Vec2,
		this.m_localYAxis1 = new d.common.math.B2Vec2,
		this.m_axis = new d.common.math.B2Vec2,
		this.m_perp = new d.common.math.B2Vec2,
		this.m_K = new d.common.math.B2Mat33,
		this.m_impulse = new d.common.math.B2Vec3;
		var t, n, r;
		this.m_localAnchor1.setV(e.localAnchorA),
		this.m_localAnchor2.setV(e.localAnchorB),
		this.m_localXAxis1.setV(e.localAxisA),
		this.m_localYAxis1.x = -this.m_localXAxis1.y,
		this.m_localYAxis1.y = this.m_localXAxis1.x,
		this.m_refAngle = e.referenceAngle,
		this.m_impulse.setZero(),
		this.m_motorMass = 0,
		this.m_motorImpulse = 0,
		this.m_lowerTranslation = e.lowerTranslation,
		this.m_upperTranslation = e.upperTranslation,
		this.m_maxMotorForce = e.maxMotorForce,
		this.m_motorSpeed = e.motorSpeed,
		this.m_enableLimit = e.enableLimit,
		this.m_enableMotor = e.enableMotor,
		this.m_limitState = d.dynamics.joints.B2Joint.e_inactiveLimit,
		this.m_axis.setZero(),
		this.m_perp.setZero()
	},
	d.dynamics.joints.B2PrismaticJoint.__name__ = !0,
	d.dynamics.joints.B2PrismaticJoint.__super__ = d.dynamics.joints.B2Joint,
	d.dynamics.joints.B2PrismaticJoint.prototype = t(d.dynamics.joints.B2Joint.prototype, {
		solvePositionConstraints: function (e) {
			var t, n, r = this.m_bodyA,
			i = this.m_bodyB,
			s = r.m_sweep.c,
			o = r.m_sweep.a,
			u = i.m_sweep.c,
			a = i.m_sweep.a,
			f, l, c, h, p, v, m = 0,
			g = 0,
			y = !1,
			b = 0,
			w = d.common.math.B2Mat22.fromAngle(o),
			E = d.common.math.B2Mat22.fromAngle(a);
			f = w;
			var S = this.m_localAnchor1.x - this.m_localCenterA.x,
			x = this.m_localAnchor1.y - this.m_localCenterA.y;
			l = f.col1.x * S + f.col2.x * x,
			x = f.col1.y * S + f.col2.y * x,
			S = l,
			f = E;
			var T = this.m_localAnchor2.x - this.m_localCenterB.x,
			N = this.m_localAnchor2.y - this.m_localCenterB.y;
			l = f.col1.x * T + f.col2.x * N,
			N = f.col1.y * T + f.col2.y * N,
			T = l;
			var C = u.x + T - s.x - S,
			k = u.y + N - s.y - x;
			if (this.m_enableLimit) {
				this.m_axis = d.common.math.B2Math.mulMV(w, this.m_localXAxis1),
				this.m_a1 = (C + S) * this.m_axis.y - (k + x) * this.m_axis.x,
				this.m_a2 = T * this.m_axis.y - N * this.m_axis.x;
				var L = this.m_axis.x * C + this.m_axis.y * k;
				d.common.math.B2Math.abs(this.m_upperTranslation - this.m_lowerTranslation) < 2 * d.common.B2Settings.b2_linearSlop ? (b = d.common.math.B2Math.clamp(L, -d.common.B2Settings.b2_maxLinearCorrection, d.common.B2Settings.b2_maxLinearCorrection), m = d.common.math.B2Math.abs(L), y = !0) : L <= this.m_lowerTranslation ? (b = d.common.math.B2Math.clamp(L - this.m_lowerTranslation + d.common.B2Settings.b2_linearSlop, -d.common.B2Settings.b2_maxLinearCorrection, 0), m = this.m_lowerTranslation - L, y = !0) : L >= this.m_upperTranslation && (b = d.common.math.B2Math.clamp(L - this.m_upperTranslation + d.common.B2Settings.b2_linearSlop, 0, d.common.B2Settings.b2_maxLinearCorrection), m = L - this.m_upperTranslation, y = !0)
			}
			this.m_perp = d.common.math.B2Math.mulMV(w, this.m_localYAxis1),
			this.m_s1 = (C + S) * this.m_perp.y - (k + x) * this.m_perp.x,
			this.m_s2 = T * this.m_perp.y - N * this.m_perp.x;
			var A = new d.common.math.B2Vec3,
			O = this.m_perp.x * C + this.m_perp.y * k,
			M = a - o - this.m_refAngle;
			m = d.common.math.B2Math.max(m, d.common.math.B2Math.abs(O)),
			g = d.common.math.B2Math.abs(M);
			if (y) c = this.m_invMassA,
			h = this.m_invMassB,
			p = this.m_invIA,
			v = this.m_invIB,
			this.m_K.col1.x = c + h + p * this.m_s1 * this.m_s1 + v * this.m_s2 * this.m_s2,
			this.m_K.col1.y = p * this.m_s1 + v * this.m_s2,
			this.m_K.col1.z = p * this.m_s1 * this.m_a1 + v * this.m_s2 * this.m_a2,
			this.m_K.col2.x = this.m_K.col1.y,
			this.m_K.col2.y = p + v,
			this.m_K.col2.z = p * this.m_a1 + v * this.m_a2,
			this.m_K.col3.x = this.m_K.col1.z,
			this.m_K.col3.y = this.m_K.col2.z,
			this.m_K.col3.z = c + h + p * this.m_a1 * this.m_a1 + v * this.m_a2 * this.m_a2,
			this.m_K.solve33(A, -O, -M, -b);
			else {
				c = this.m_invMassA,
				h = this.m_invMassB,
				p = this.m_invIA,
				v = this.m_invIB;
				var _ = c + h + p * this.m_s1 * this.m_s1 + v * this.m_s2 * this.m_s2,
				D = p * this.m_s1 + v * this.m_s2,
				P = p + v;
				this.m_K.col1.set(_, D, 0),
				this.m_K.col2.set(D, P, 0);
				var H = this.m_K.solve22(new d.common.math.B2Vec2, -O, -M);
				A.x = H.x,
				A.y = H.y,
				A.z = 0
			}
			var B = A.x * this.m_perp.x + A.z * this.m_axis.x,
			j = A.x * this.m_perp.y + A.z * this.m_axis.y,
			F = A.x * this.m_s1 + A.y + A.z * this.m_a1,
			I = A.x * this.m_s2 + A.y + A.z * this.m_a2;
			return s.x -= this.m_invMassA * B,
			s.y -= this.m_invMassA * j,
			o -= this.m_invIA * F,
			u.x += this.m_invMassB * B,
			u.y += this.m_invMassB * j,
			a += this.m_invIB * I,
			r.m_sweep.a = o,
			i.m_sweep.a = a,
			r.synchronizeTransform(),
			i.synchronizeTransform(),
			m <= d.common.B2Settings.b2_linearSlop && g <= d.common.B2Settings.b2_angularSlop
		},
		solveVelocityConstraints: function (e) {
			var t = this.m_bodyA,
			n = this.m_bodyB,
			r = t.m_linearVelocity,
			i = t.m_angularVelocity,
			s = n.m_linearVelocity,
			o = n.m_angularVelocity,
			u, a, f, l;
			if (this.m_enableMotor && this.m_limitState != d.dynamics.joints.B2Joint.e_equalLimits) {
				var c = this.m_axis.x * (s.x - r.x) + this.m_axis.y * (s.y - r.y) + this.m_a2 * o - this.m_a1 * i,
				h = this.m_motorMass * (this.m_motorSpeed - c),
				p = this.m_motorImpulse,
				v = e.dt * this.m_maxMotorForce;
				this.m_motorImpulse = d.common.math.B2Math.clamp(this.m_motorImpulse + h, -v, v),
				h = this.m_motorImpulse - p,
				u = h * this.m_axis.x,
				a = h * this.m_axis.y,
				f = h * this.m_a1,
				l = h * this.m_a2,
				r.x -= this.m_invMassA * u,
				r.y -= this.m_invMassA * a,
				i -= this.m_invIA * f,
				s.x += this.m_invMassB * u,
				s.y += this.m_invMassB * a,
				o += this.m_invIB * l
			}
			var m = this.m_perp.x * (s.x - r.x) + this.m_perp.y * (s.y - r.y) + this.m_s2 * o - this.m_s1 * i,
			g = o - i;
			if (this.m_enableLimit && this.m_limitState != d.dynamics.joints.B2Joint.e_inactiveLimit) {
				var y = this.m_axis.x * (s.x - r.x) + this.m_axis.y * (s.y - r.y) + this.m_a2 * o - this.m_a1 * i,
				b = this.m_impulse.copy(),
				w = this.m_K.solve33(new d.common.math.B2Vec3, -m, -g, -y);
				this.m_impulse.add(w),
				this.m_limitState == d.dynamics.joints.B2Joint.e_atLowerLimit ? this.m_impulse.z = d.common.math.B2Math.max(this.m_impulse.z, 0) : this.m_limitState == d.dynamics.joints.B2Joint.e_atUpperLimit && (this.m_impulse.z = d.common.math.B2Math.min(this.m_impulse.z, 0));
				var E = -m - (this.m_impulse.z - b.z) * this.m_K.col3.x,
				S = -g - (this.m_impulse.z - b.z) * this.m_K.col3.y,
				x = this.m_K.solve22(new d.common.math.B2Vec2, E, S);
				x.x += b.x,
				x.y += b.y,
				this.m_impulse.x = x.x,
				this.m_impulse.y = x.y,
				w.x = this.m_impulse.x - b.x,
				w.y = this.m_impulse.y - b.y,
				w.z = this.m_impulse.z - b.z,
				u = w.x * this.m_perp.x + w.z * this.m_axis.x,
				a = w.x * this.m_perp.y + w.z * this.m_axis.y,
				f = w.x * this.m_s1 + w.y + w.z * this.m_a1,
				l = w.x * this.m_s2 + w.y + w.z * this.m_a2,
				r.x -= this.m_invMassA * u,
				r.y -= this.m_invMassA * a,
				i -= this.m_invIA * f,
				s.x += this.m_invMassB * u,
				s.y += this.m_invMassB * a,
				o += this.m_invIB * l
			} else {
				var T = this.m_K.solve22(new d.common.math.B2Vec2, -m, -g);
				this.m_impulse.x += T.x,
				this.m_impulse.y += T.y,
				u = T.x * this.m_perp.x,
				a = T.x * this.m_perp.y,
				f = T.x * this.m_s1 + T.y,
				l = T.x * this.m_s2 + T.y,
				r.x -= this.m_invMassA * u,
				r.y -= this.m_invMassA * a,
				i -= this.m_invIA * f,
				s.x += this.m_invMassB * u,
				s.y += this.m_invMassB * a,
				o += this.m_invIB * l
			}
			t.m_linearVelocity.setV(r),
			t.m_angularVelocity = i,
			n.m_linearVelocity.setV(s),
			n.m_angularVelocity = o
		},
		initVelocityConstraints: function (e) {
			var t = this.m_bodyA,
			n = this.m_bodyB,
			r, i;
			this.m_localCenterA.setV(t.getLocalCenter()),
			this.m_localCenterB.setV(n.getLocalCenter());
			var s = t.getTransform(),
			o = n.getTransform();
			r = t.m_xf.R;
			var u = this.m_localAnchor1.x - this.m_localCenterA.x,
			a = this.m_localAnchor1.y - this.m_localCenterA.y;
			i = r.col1.x * u + r.col2.x * a,
			a = r.col1.y * u + r.col2.y * a,
			u = i,
			r = n.m_xf.R;
			var f = this.m_localAnchor2.x - this.m_localCenterB.x,
			l = this.m_localAnchor2.y - this.m_localCenterB.y;
			i = r.col1.x * f + r.col2.x * l,
			l = r.col1.y * f + r.col2.y * l,
			f = i;
			var c = n.m_sweep.c.x + f - t.m_sweep.c.x - u,
			h = n.m_sweep.c.y + l - t.m_sweep.c.y - a;
			this.m_invMassA = t.m_invMass,
			this.m_invMassB = n.m_invMass,
			this.m_invIA = t.m_invI,
			this.m_invIB = n.m_invI,
			this.m_axis.setV(d.common.math.B2Math.mulMV(s.R, this.m_localXAxis1)),
			this.m_a1 = (c + u) * this.m_axis.y - (h + a) * this.m_axis.x,
			this.m_a2 = f * this.m_axis.y - l * this.m_axis.x,
			this.m_motorMass = this.m_invMassA + this.m_invMassB + this.m_invIA * this.m_a1 * this.m_a1 + this.m_invIB * this.m_a2 * this.m_a2,
			this.m_motorMass > 2.2250738585072014e-308 && (this.m_motorMass = 1 / this.m_motorMass),
			this.m_perp.setV(d.common.math.B2Math.mulMV(s.R, this.m_localYAxis1)),
			this.m_s1 = (c + u) * this.m_perp.y - (h + a) * this.m_perp.x,
			this.m_s2 = f * this.m_perp.y - l * this.m_perp.x;
			var p = this.m_invMassA,
			v = this.m_invMassB,
			m = this.m_invIA,
			g = this.m_invIB;
			this.m_K.col1.x = p + v + m * this.m_s1 * this.m_s1 + g * this.m_s2 * this.m_s2,
			this.m_K.col1.y = m * this.m_s1 + g * this.m_s2,
			this.m_K.col1.z = m * this.m_s1 * this.m_a1 + g * this.m_s2 * this.m_a2,
			this.m_K.col2.x = this.m_K.col1.y,
			this.m_K.col2.y = m + g,
			this.m_K.col2.z = m * this.m_a1 + g * this.m_a2,
			this.m_K.col3.x = this.m_K.col1.z,
			this.m_K.col3.y = this.m_K.col2.z,
			this.m_K.col3.z = p + v + m * this.m_a1 * this.m_a1 + g * this.m_a2 * this.m_a2;
			if (this.m_enableLimit) {
				var y = this.m_axis.x * c + this.m_axis.y * h;
				d.common.math.B2Math.abs(this.m_upperTranslation - this.m_lowerTranslation) < 2 * d.common.B2Settings.b2_linearSlop ? this.m_limitState = d.dynamics.joints.B2Joint.e_equalLimits: y <= this.m_lowerTranslation ? this.m_limitState != d.dynamics.joints.B2Joint.e_atLowerLimit && (this.m_limitState = d.dynamics.joints.B2Joint.e_atLowerLimit, this.m_impulse.z = 0) : y >= this.m_upperTranslation ? this.m_limitState != d.dynamics.joints.B2Joint.e_atUpperLimit && (this.m_limitState = d.dynamics.joints.B2Joint.e_atUpperLimit, this.m_impulse.z = 0) : (this.m_limitState = d.dynamics.joints.B2Joint.e_inactiveLimit, this.m_impulse.z = 0)
			} else this.m_limitState = d.dynamics.joints.B2Joint.e_inactiveLimit;
			this.m_enableMotor == 0 && (this.m_motorImpulse = 0);
			if (e.warmStarting) {
				this.m_impulse.x *= e.dtRatio,
				this.m_impulse.y *= e.dtRatio,
				this.m_motorImpulse *= e.dtRatio;
				var b = this.m_impulse.x * this.m_perp.x + (this.m_motorImpulse + this.m_impulse.z) * this.m_axis.x,
				w = this.m_impulse.x * this.m_perp.y + (this.m_motorImpulse + this.m_impulse.z) * this.m_axis.y,
				E = this.m_impulse.x * this.m_s1 + this.m_impulse.y + (this.m_motorImpulse + this.m_impulse.z) * this.m_a1,
				S = this.m_impulse.x * this.m_s2 + this.m_impulse.y + (this.m_motorImpulse + this.m_impulse.z) * this.m_a2;
				t.m_linearVelocity.x -= this.m_invMassA * b,
				t.m_linearVelocity.y -= this.m_invMassA * w,
				t.m_angularVelocity -= this.m_invIA * E,
				n.m_linearVelocity.x += this.m_invMassB * b,
				n.m_linearVelocity.y += this.m_invMassB * w,
				n.m_angularVelocity += this.m_invIB * S
			} else this.m_impulse.setZero(),
			this.m_motorImpulse = 0
		},
		getMotorForce: function () {
			return this.m_motorImpulse
		},
		setMaxMotorForce: function (e) {
			this.m_bodyA.setAwake(!0),
			this.m_bodyB.setAwake(!0),
			this.m_maxMotorForce = e
		},
		getMotorSpeed: function () {
			return this.m_motorSpeed
		},
		setMotorSpeed: function (e) {
			this.m_bodyA.setAwake(!0),
			this.m_bodyB.setAwake(!0),
			this.m_motorSpeed = e
		},
		enableMotor: function (e) {
			this.m_bodyA.setAwake(!0),
			this.m_bodyB.setAwake(!0),
			this.m_enableMotor = e
		},
		isMotorEnabled: function () {
			return this.m_enableMotor
		},
		setLimits: function (e, t) {
			this.m_bodyA.setAwake(!0),
			this.m_bodyB.setAwake(!0),
			this.m_lowerTranslation = e,
			this.m_upperTranslation = t
		},
		getUpperLimit: function () {
			return this.m_upperTranslation
		},
		getLowerLimit: function () {
			return this.m_lowerTranslation
		},
		enableLimit: function (e) {
			this.m_bodyA.setAwake(!0),
			this.m_bodyB.setAwake(!0),
			this.m_enableLimit = e
		},
		isLimitEnabled: function () {
			return this.m_enableLimit
		},
		getJointSpeed: function () {
			var e = this.m_bodyA,
			t = this.m_bodyB,
			n;
			n = e.m_xf.R;
			var r = this.m_localAnchor1.x - e.m_sweep.localCenter.x,
			i = this.m_localAnchor1.y - e.m_sweep.localCenter.y,
			s = n.col1.x * r + n.col2.x * i;
			i = n.col1.y * r + n.col2.y * i,
			r = s,
			n = t.m_xf.R;
			var o = this.m_localAnchor2.x - t.m_sweep.localCenter.x,
			u = this.m_localAnchor2.y - t.m_sweep.localCenter.y;
			s = n.col1.x * o + n.col2.x * u,
			u = n.col1.y * o + n.col2.y * u,
			o = s;
			var a = e.m_sweep.c.x + r,
			f = e.m_sweep.c.y + i,
			l = t.m_sweep.c.x + o,
			c = t.m_sweep.c.y + u,
			h = l - a,
			p = c - f,
			d = e.getWorldVector(this.m_localXAxis1),
			v = e.m_linearVelocity,
			m = t.m_linearVelocity,
			g = e.m_angularVelocity,
			y = t.m_angularVelocity,
			b = h * -g * d.y + p * g * d.x + (d.x * (m.x + -y * u - v.x - -g * i) + d.y * (m.y + y * o - v.y - g * r));
			return b
		},
		getJointTranslation: function () {
			var e = this.m_bodyA,
			t = this.m_bodyB,
			n, r = e.getWorldPoint(this.m_localAnchor1),
			i = t.getWorldPoint(this.m_localAnchor2),
			s = i.x - r.x,
			o = i.y - r.y,
			u = e.getWorldVector(this.m_localXAxis1),
			a = u.x * s + u.y * o;
			return a
		},
		getReactionTorque: function (e) {
			return e * this.m_impulse.y
		},
		getReactionForce: function (e) {
			return new d.common.math.B2Vec2(e * (this.m_impulse.x * this.m_perp.x + (this.m_motorImpulse + this.m_impulse.z) * this.m_axis.x), e * (this.m_impulse.x * this.m_perp.y + (this.m_motorImpulse + this.m_impulse.z) * this.m_axis.y))
		},
		getAnchorB: function () {
			return this.m_bodyB.getWorldPoint(this.m_localAnchor2)
		},
		getAnchorA: function () {
			return this.m_bodyA.getWorldPoint(this.m_localAnchor1)
		},
		__class__: d.dynamics.joints.B2PrismaticJoint
	}),
	d.dynamics.joints.B2PrismaticJointDef = function () {
		d.dynamics.joints.B2JointDef.call(this),
		this.localAnchorA = new d.common.math.B2Vec2,
		this.localAnchorB = new d.common.math.B2Vec2,
		this.localAxisA = new d.common.math.B2Vec2,
		this.type = d.dynamics.joints.B2Joint.e_prismaticJoint,
		this.localAxisA.set(1, 0),
		this.referenceAngle = 0,
		this.enableLimit = !1,
		this.lowerTranslation = 0,
		this.upperTranslation = 0,
		this.enableMotor = !1,
		this.maxMotorForce = 0,
		this.motorSpeed = 0
	},
	d.dynamics.joints.B2PrismaticJointDef.__name__ = !0,
	d.dynamics.joints.B2PrismaticJointDef.__super__ = d.dynamics.joints.B2JointDef,
	d.dynamics.joints.B2PrismaticJointDef.prototype = t(d.dynamics.joints.B2JointDef.prototype, {
		initialize: function (e, t, n, r) {
			this.bodyA = e,
			this.bodyB = t,
			this.localAnchorA = this.bodyA.getLocalPoint(n),
			this.localAnchorB = this.bodyB.getLocalPoint(n),
			this.localAxisA = this.bodyA.getLocalVector(r),
			this.referenceAngle = this.bodyB.getAngle() - this.bodyA.getAngle()
		},
		__class__: d.dynamics.joints.B2PrismaticJointDef
	}),
	d.dynamics.joints.B2PulleyJoint = function (e) {
		d.dynamics.joints.B2Joint.call(this, e),
		this.m_groundAnchor1 = new d.common.math.B2Vec2,
		this.m_groundAnchor2 = new d.common.math.B2Vec2,
		this.m_localAnchor1 = new d.common.math.B2Vec2,
		this.m_localAnchor2 = new d.common.math.B2Vec2,
		this.m_u1 = new d.common.math.B2Vec2,
		this.m_u2 = new d.common.math.B2Vec2;
		var t, n, r;
		this.m_ground = this.m_bodyA.m_world.m_groundBody,
		this.m_groundAnchor1.x = e.groundAnchorA.x - this.m_ground.m_xf.position.x,
		this.m_groundAnchor1.y = e.groundAnchorA.y - this.m_ground.m_xf.position.y,
		this.m_groundAnchor2.x = e.groundAnchorB.x - this.m_ground.m_xf.position.x,
		this.m_groundAnchor2.y = e.groundAnchorB.y - this.m_ground.m_xf.position.y,
		this.m_localAnchor1.setV(e.localAnchorA),
		this.m_localAnchor2.setV(e.localAnchorB),
		this.m_ratio = e.ratio,
		this.m_constant = e.lengthA + this.m_ratio * e.lengthB,
		this.m_maxLength1 = d.common.math.B2Math.min(e.maxLengthA, this.m_constant - this.m_ratio * d.dynamics.joints.B2PulleyJoint.b2_minPulleyLength),
		this.m_maxLength2 = d.common.math.B2Math.min(e.maxLengthB, (this.m_constant - d.dynamics.joints.B2PulleyJoint.b2_minPulleyLength) / this.m_ratio),
		this.m_impulse = 0,
		this.m_limitImpulse1 = 0,
		this.m_limitImpulse2 = 0
	},
	d.dynamics.joints.B2PulleyJoint.__name__ = !0,
	d.dynamics.joints.B2PulleyJoint.__super__ = d.dynamics.joints.B2Joint,
	d.dynamics.joints.B2PulleyJoint.prototype = t(d.dynamics.joints.B2Joint.prototype, {
		solvePositionConstraints: function (e) {
			var t = this.m_bodyA,
			n = this.m_bodyB,
			r, i = this.m_ground.m_xf.position.x + this.m_groundAnchor1.x,
			s = this.m_ground.m_xf.position.y + this.m_groundAnchor1.y,
			o = this.m_ground.m_xf.position.x + this.m_groundAnchor2.x,
			u = this.m_ground.m_xf.position.y + this.m_groundAnchor2.y,
			a, f, l, c, h, p, v, m, g, y, b, w, E, S, x, T = 0;
			return this.m_state == d.dynamics.joints.B2Joint.e_atUpperLimit && (r = t.m_xf.R, a = this.m_localAnchor1.x - t.m_sweep.localCenter.x, f = this.m_localAnchor1.y - t.m_sweep.localCenter.y, x = r.col1.x * a + r.col2.x * f, f = r.col1.y * a + r.col2.y * f, a = x, r = n.m_xf.R, l = this.m_localAnchor2.x - n.m_sweep.localCenter.x, c = this.m_localAnchor2.y - n.m_sweep.localCenter.y, x = r.col1.x * l + r.col2.x * c, c = r.col1.y * l + r.col2.y * c, l = x, h = t.m_sweep.c.x + a, p = t.m_sweep.c.y + f, v = n.m_sweep.c.x + l, m = n.m_sweep.c.y + c, this.m_u1.set(h - i, p - s), this.m_u2.set(v - o, m - u), g = this.m_u1.length(), y = this.m_u2.length(), g > d.common.B2Settings.b2_linearSlop ? this.m_u1.multiply(1 / g) : this.m_u1.setZero(), y > d.common.B2Settings.b2_linearSlop ? this.m_u2.multiply(1 / y) : this.m_u2.setZero(), b = this.m_constant - g - this.m_ratio * y, T = d.common.math.B2Math.max(T, -b), b = d.common.math.B2Math.clamp(b + d.common.B2Settings.b2_linearSlop, -d.common.B2Settings.b2_maxLinearCorrection, 0), w = -this.m_pulleyMass * b, h = -w * this.m_u1.x, p = -w * this.m_u1.y, v = -this.m_ratio * w * this.m_u2.x, m = -this.m_ratio * w * this.m_u2.y, t.m_sweep.c.x += t.m_invMass * h, t.m_sweep.c.y += t.m_invMass * p, t.m_sweep.a += t.m_invI * (a * p - f * h), n.m_sweep.c.x += n.m_invMass * v, n.m_sweep.c.y += n.m_invMass * m, n.m_sweep.a += n.m_invI * (l * m - c * v), t.synchronizeTransform(), n.synchronizeTransform()),
			this.m_limitState1 == d.dynamics.joints.B2Joint.e_atUpperLimit && (r = t.m_xf.R, a = this.m_localAnchor1.x - t.m_sweep.localCenter.x, f = this.m_localAnchor1.y - t.m_sweep.localCenter.y, x = r.col1.x * a + r.col2.x * f, f = r.col1.y * a + r.col2.y * f, a = x, h = t.m_sweep.c.x + a, p = t.m_sweep.c.y + f, this.m_u1.set(h - i, p - s), g = this.m_u1.length(), g > d.common.B2Settings.b2_linearSlop ? (this.m_u1.x *= 1 / g, this.m_u1.y *= 1 / g) : this.m_u1.setZero(), b = this.m_maxLength1 - g, T = d.common.math.B2Math.max(T, -b), b = d.common.math.B2Math.clamp(b + d.common.B2Settings.b2_linearSlop, -d.common.B2Settings.b2_maxLinearCorrection, 0), w = -this.m_limitMass1 * b, h = -w * this.m_u1.x, p = -w * this.m_u1.y, t.m_sweep.c.x += t.m_invMass * h, t.m_sweep.c.y += t.m_invMass * p, t.m_sweep.a += t.m_invI * (a * p - f * h), t.synchronizeTransform()),
			this.m_limitState2 == d.dynamics.joints.B2Joint.e_atUpperLimit && (r = n.m_xf.R, l = this.m_localAnchor2.x - n.m_sweep.localCenter.x, c = this.m_localAnchor2.y - n.m_sweep.localCenter.y, x = r.col1.x * l + r.col2.x * c, c = r.col1.y * l + r.col2.y * c, l = x, v = n.m_sweep.c.x + l, m = n.m_sweep.c.y + c, this.m_u2.set(v - o, m - u), y = this.m_u2.length(), y > d.common.B2Settings.b2_linearSlop ? (this.m_u2.x *= 1 / y, this.m_u2.y *= 1 / y) : this.m_u2.setZero(), b = this.m_maxLength2 - y, T = d.common.math.B2Math.max(T, -b), b = d.common.math.B2Math.clamp(b + d.common.B2Settings.b2_linearSlop, -d.common.B2Settings.b2_maxLinearCorrection, 0), w = -this.m_limitMass2 * b, v = -w * this.m_u2.x, m = -w * this.m_u2.y, n.m_sweep.c.x += n.m_invMass * v, n.m_sweep.c.y += n.m_invMass * m, n.m_sweep.a += n.m_invI * (l * m - c * v), n.synchronizeTransform()),
			T < d.common.B2Settings.b2_linearSlop
		},
		solveVelocityConstraints: function (e) {
			var t = this.m_bodyA,
			n = this.m_bodyB,
			r;
			r = t.m_xf.R;
			var i = this.m_localAnchor1.x - t.m_sweep.localCenter.x,
			s = this.m_localAnchor1.y - t.m_sweep.localCenter.y,
			o = r.col1.x * i + r.col2.x * s;
			s = r.col1.y * i + r.col2.y * s,
			i = o,
			r = n.m_xf.R;
			var u = this.m_localAnchor2.x - n.m_sweep.localCenter.x,
			a = this.m_localAnchor2.y - n.m_sweep.localCenter.y;
			o = r.col1.x * u + r.col2.x * a,
			a = r.col1.y * u + r.col2.y * a,
			u = o;
			var f, l, c, h, p, v, m, g, y, b, w;
			this.m_state == d.dynamics.joints.B2Joint.e_atUpperLimit && (f = t.m_linearVelocity.x + -t.m_angularVelocity * s, l = t.m_linearVelocity.y + t.m_angularVelocity * i, c = n.m_linearVelocity.x + -n.m_angularVelocity * a, h = n.m_linearVelocity.y + n.m_angularVelocity * u, y = -(this.m_u1.x * f + this.m_u1.y * l) - this.m_ratio * (this.m_u2.x * c + this.m_u2.y * h), b = this.m_pulleyMass * -y, w = this.m_impulse, this.m_impulse = d.common.math.B2Math.max(0, this.m_impulse + b), b = this.m_impulse - w, p = -b * this.m_u1.x, v = -b * this.m_u1.y, m = -this.m_ratio * b * this.m_u2.x, g = -this.m_ratio * b * this.m_u2.y, t.m_linearVelocity.x += t.m_invMass * p, t.m_linearVelocity.y += t.m_invMass * v, t.m_angularVelocity += t.m_invI * (i * v - s * p), n.m_linearVelocity.x += n.m_invMass * m, n.m_linearVelocity.y += n.m_invMass * g, n.m_angularVelocity += n.m_invI * (u * g - a * m)),
			this.m_limitState1 == d.dynamics.joints.B2Joint.e_atUpperLimit && (f = t.m_linearVelocity.x + -t.m_angularVelocity * s, l = t.m_linearVelocity.y + t.m_angularVelocity * i, y = -(this.m_u1.x * f + this.m_u1.y * l), b = -this.m_limitMass1 * y, w = this.m_limitImpulse1, this.m_limitImpulse1 = d.common.math.B2Math.max(0, this.m_limitImpulse1 + b), b = this.m_limitImpulse1 - w, p = -b * this.m_u1.x, v = -b * this.m_u1.y, t.m_linearVelocity.x += t.m_invMass * p, t.m_linearVelocity.y += t.m_invMass * v, t.m_angularVelocity += t.m_invI * (i * v - s * p)),
			this.m_limitState2 == d.dynamics.joints.B2Joint.e_atUpperLimit && (c = n.m_linearVelocity.x + -n.m_angularVelocity * a, h = n.m_linearVelocity.y + n.m_angularVelocity * u, y = -(this.m_u2.x * c + this.m_u2.y * h), b = -this.m_limitMass2 * y, w = this.m_limitImpulse2, this.m_limitImpulse2 = d.common.math.B2Math.max(0, this.m_limitImpulse2 + b), b = this.m_limitImpulse2 - w, m = -b * this.m_u2.x, g = -b * this.m_u2.y, n.m_linearVelocity.x += n.m_invMass * m, n.m_linearVelocity.y += n.m_invMass * g, n.m_angularVelocity += n.m_invI * (u * g - a * m))
		},
		initVelocityConstraints: function (e) {
			var t = this.m_bodyA,
			n = this.m_bodyB,
			r;
			r = t.m_xf.R;
			var i = this.m_localAnchor1.x - t.m_sweep.localCenter.x,
			s = this.m_localAnchor1.y - t.m_sweep.localCenter.y,
			o = r.col1.x * i + r.col2.x * s;
			s = r.col1.y * i + r.col2.y * s,
			i = o,
			r = n.m_xf.R;
			var u = this.m_localAnchor2.x - n.m_sweep.localCenter.x,
			a = this.m_localAnchor2.y - n.m_sweep.localCenter.y;
			o = r.col1.x * u + r.col2.x * a,
			a = r.col1.y * u + r.col2.y * a,
			u = o;
			var f = t.m_sweep.c.x + i,
			l = t.m_sweep.c.y + s,
			c = n.m_sweep.c.x + u,
			h = n.m_sweep.c.y + a,
			p = this.m_ground.m_xf.position.x + this.m_groundAnchor1.x,
			v = this.m_ground.m_xf.position.y + this.m_groundAnchor1.y,
			m = this.m_ground.m_xf.position.x + this.m_groundAnchor2.x,
			g = this.m_ground.m_xf.position.y + this.m_groundAnchor2.y;
			this.m_u1.set(f - p, l - v),
			this.m_u2.set(c - m, h - g);
			var y = this.m_u1.length(),
			b = this.m_u2.length();
			y > d.common.B2Settings.b2_linearSlop ? this.m_u1.multiply(1 / y) : this.m_u1.setZero(),
			b > d.common.B2Settings.b2_linearSlop ? this.m_u2.multiply(1 / b) : this.m_u2.setZero();
			var w = this.m_constant - y - this.m_ratio * b;
			w > 0 ? (this.m_state = d.dynamics.joints.B2Joint.e_inactiveLimit, this.m_impulse = 0) : this.m_state = d.dynamics.joints.B2Joint.e_atUpperLimit,
			y < this.m_maxLength1 ? (this.m_limitState1 = d.dynamics.joints.B2Joint.e_inactiveLimit, this.m_limitImpulse1 = 0) : this.m_limitState1 = d.dynamics.joints.B2Joint.e_atUpperLimit,
			b < this.m_maxLength2 ? (this.m_limitState2 = d.dynamics.joints.B2Joint.e_inactiveLimit, this.m_limitImpulse2 = 0) : this.m_limitState2 = d.dynamics.joints.B2Joint.e_atUpperLimit;
			var E = i * this.m_u1.y - s * this.m_u1.x,
			S = u * this.m_u2.y - a * this.m_u2.x;
			this.m_limitMass1 = t.m_invMass + t.m_invI * E * E,
			this.m_limitMass2 = n.m_invMass + n.m_invI * S * S,
			this.m_pulleyMass = this.m_limitMass1 + this.m_ratio * this.m_ratio * this.m_limitMass2,
			this.m_limitMass1 = 1 / this.m_limitMass1,
			this.m_limitMass2 = 1 / this.m_limitMass2,
			this.m_pulleyMass = 1 / this.m_pulleyMass;
			if (e.warmStarting) {
				this.m_impulse *= e.dtRatio,
				this.m_limitImpulse1 *= e.dtRatio,
				this.m_limitImpulse2 *= e.dtRatio;
				var x = ( - this.m_impulse - this.m_limitImpulse1) * this.m_u1.x,
				T = ( - this.m_impulse - this.m_limitImpulse1) * this.m_u1.y,
				N = ( - this.m_ratio * this.m_impulse - this.m_limitImpulse2) * this.m_u2.x,
				C = ( - this.m_ratio * this.m_impulse - this.m_limitImpulse2) * this.m_u2.y;
				t.m_linearVelocity.x += t.m_invMass * x,
				t.m_linearVelocity.y += t.m_invMass * T,
				t.m_angularVelocity += t.m_invI * (i * T - s * x),
				n.m_linearVelocity.x += n.m_invMass * N,
				n.m_linearVelocity.y += n.m_invMass * C,
				n.m_angularVelocity += n.m_invI * (u * C - a * N)
			} else this.m_impulse = 0,
			this.m_limitImpulse1 = 0,
			this.m_limitImpulse2 = 0
		},
		getRatio: function () {
			return this.m_ratio
		},
		getLength2: function () {
			var e = this.m_bodyB.getWorldPoint(this.m_localAnchor2),
			t = this.m_ground.m_xf.position.x + this.m_groundAnchor2.x,
			n = this.m_ground.m_xf.position.y + this.m_groundAnchor2.y,
			r = e.x - t,
			i = e.y - n;
			return Math.sqrt(r * r + i * i)
		},
		getLength1: function () {
			var e = this.m_bodyA.getWorldPoint(this.m_localAnchor1),
			t = this.m_ground.m_xf.position.x + this.m_groundAnchor1.x,
			n = this.m_ground.m_xf.position.y + this.m_groundAnchor1.y,
			r = e.x - t,
			i = e.y - n;
			return Math.sqrt(r * r + i * i)
		},
		getGroundAnchorB: function () {
			var e = this.m_ground.m_xf.position.copy();
			return e.add(this.m_groundAnchor2),
			e
		},
		getGroundAnchorA: function () {
			var e = this.m_ground.m_xf.position.copy();
			return e.add(this.m_groundAnchor1),
			e
		},
		getReactionTorque: function (e) {
			return 0
		},
		getReactionForce: function (e) {
			return new d.common.math.B2Vec2(e * this.m_impulse * this.m_u2.x, e * this.m_impulse * this.m_u2.y)
		},
		getAnchorB: function () {
			return this.m_bodyB.getWorldPoint(this.m_localAnchor2)
		},
		getAnchorA: function () {
			return this.m_bodyA.getWorldPoint(this.m_localAnchor1)
		},
		__class__: d.dynamics.joints.B2PulleyJoint
	}),
	d.dynamics.joints.B2PulleyJointDef = function () {
		d.dynamics.joints.B2JointDef.call(this),
		this.groundAnchorA = new d.common.math.B2Vec2,
		this.groundAnchorB = new d.common.math.B2Vec2,
		this.localAnchorA = new d.common.math.B2Vec2,
		this.localAnchorB = new d.common.math.B2Vec2,
		this.type = d.dynamics.joints.B2Joint.e_pulleyJoint,
		this.groundAnchorA.set( - 1, 1),
		this.groundAnchorB.set(1, 1),
		this.localAnchorA.set( - 1, 0),
		this.localAnchorB.set(1, 0),
		this.lengthA = 0,
		this.maxLengthA = 0,
		this.lengthB = 0,
		this.maxLengthB = 0,
		this.ratio = 1,
		this.collideConnected = !0
	},
	d.dynamics.joints.B2PulleyJointDef.__name__ = !0,
	d.dynamics.joints.B2PulleyJointDef.__super__ = d.dynamics.joints.B2JointDef,
	d.dynamics.joints.B2PulleyJointDef.prototype = t(d.dynamics.joints.B2JointDef.prototype, {
		initialize: function (e, t, n, r, i, s, o) {
			this.bodyA = e,
			this.bodyB = t,
			this.groundAnchorA.setV(n),
			this.groundAnchorB.setV(r),
			this.localAnchorA = this.bodyA.getLocalPoint(i),
			this.localAnchorB = this.bodyB.getLocalPoint(s);
			var u = i.x - n.x,
			a = i.y - n.y;
			this.lengthA = Math.sqrt(u * u + a * a);
			var f = s.x - r.x,
			l = s.y - r.y;
			this.lengthB = Math.sqrt(f * f + l * l),
			this.ratio = o;
			var c = this.lengthA + this.ratio * this.lengthB;
			this.maxLengthA = c - this.ratio * d.dynamics.joints.B2PulleyJoint.b2_minPulleyLength,
			this.maxLengthB = (c - d.dynamics.joints.B2PulleyJoint.b2_minPulleyLength) / this.ratio
		},
		__class__: d.dynamics.joints.B2PulleyJointDef
	}),
	d.dynamics.joints.B2RevoluteJoint = function (e) {
		d.dynamics.joints.B2Joint.call(this, e),
		this.K = new d.common.math.B2Mat22,
		this.K1 = new d.common.math.B2Mat22,
		this.K2 = new d.common.math.B2Mat22,
		this.K3 = new d.common.math.B2Mat22,
		this.impulse3 = new d.common.math.B2Vec3,
		this.impulse2 = new d.common.math.B2Vec2,
		this.reduced = new d.common.math.B2Vec2,
		this.m_localAnchor1 = new d.common.math.B2Vec2,
		this.m_localAnchor2 = new d.common.math.B2Vec2,
		this.m_impulse = new d.common.math.B2Vec3,
		this.m_mass = new d.common.math.B2Mat33,
		this.m_localAnchor1.setV(e.localAnchorA),
		this.m_localAnchor2.setV(e.localAnchorB),
		this.m_referenceAngle = e.referenceAngle,
		this.m_impulse.setZero(),
		this.m_motorImpulse = 0,
		this.m_lowerAngle = e.lowerAngle,
		this.m_upperAngle = e.upperAngle,
		this.m_maxMotorTorque = e.maxMotorTorque,
		this.m_motorSpeed = e.motorSpeed,
		this.m_enableLimit = e.enableLimit,
		this.m_enableMotor = e.enableMotor,
		this.m_limitState = d.dynamics.joints.B2Joint.e_inactiveLimit
	},
	d.dynamics.joints.B2RevoluteJoint.__name__ = !0,
	d.dynamics.joints.B2RevoluteJoint.__super__ = d.dynamics.joints.B2Joint,
	d.dynamics.joints.B2RevoluteJoint.prototype = t(d.dynamics.joints.B2Joint.prototype, {
		solvePositionConstraints: function (e) {
			var t, n, r, i = this.m_bodyA,
			s = this.m_bodyB,
			o = 0,
			u = 0,
			a, f, l;
			if (this.m_enableLimit && this.m_limitState != d.dynamics.joints.B2Joint.e_inactiveLimit) {
				var c = s.m_sweep.a - i.m_sweep.a - this.m_referenceAngle,
				h = 0;
				this.m_limitState == d.dynamics.joints.B2Joint.e_equalLimits ? (n = d.common.math.B2Math.clamp(c - this.m_lowerAngle, -d.common.B2Settings.b2_maxAngularCorrection, d.common.B2Settings.b2_maxAngularCorrection), h = -this.m_motorMass * n, o = d.common.math.B2Math.abs(n)) : this.m_limitState == d.dynamics.joints.B2Joint.e_atLowerLimit ? (n = c - this.m_lowerAngle, o = -n, n = d.common.math.B2Math.clamp(n + d.common.B2Settings.b2_angularSlop, -d.common.B2Settings.b2_maxAngularCorrection, 0), h = -this.m_motorMass * n) : this.m_limitState == d.dynamics.joints.B2Joint.e_atUpperLimit && (n = c - this.m_upperAngle, o = n, n = d.common.math.B2Math.clamp(n - d.common.B2Settings.b2_angularSlop, 0, d.common.B2Settings.b2_maxAngularCorrection), h = -this.m_motorMass * n),
				i.m_sweep.a -= i.m_invI * h,
				s.m_sweep.a += s.m_invI * h,
				i.synchronizeTransform(),
				s.synchronizeTransform()
			}
			r = i.m_xf.R;
			var p = this.m_localAnchor1.x - i.m_sweep.localCenter.x,
			v = this.m_localAnchor1.y - i.m_sweep.localCenter.y;
			a = r.col1.x * p + r.col2.x * v,
			v = r.col1.y * p + r.col2.y * v,
			p = a,
			r = s.m_xf.R;
			var m = this.m_localAnchor2.x - s.m_sweep.localCenter.x,
			g = this.m_localAnchor2.y - s.m_sweep.localCenter.y;
			a = r.col1.x * m + r.col2.x * g,
			g = r.col1.y * m + r.col2.y * g,
			m = a;
			var y = s.m_sweep.c.x + m - i.m_sweep.c.x - p,
			b = s.m_sweep.c.y + g - i.m_sweep.c.y - v,
			w = y * y + b * b,
			E = Math.sqrt(w);
			u = E;
			var S = i.m_invMass,
			x = s.m_invMass,
			T = i.m_invI,
			N = s.m_invI,
			C = 10 * d.common.B2Settings.b2_linearSlop;
			if (w > C * C) {
				var k = y / E,
				L = b / E,
				A = S + x,
				O = 1 / A;
				f = O * -y,
				l = O * -b;
				var M = .5;
				i.m_sweep.c.x -= M * S * f,
				i.m_sweep.c.y -= M * S * l,
				s.m_sweep.c.x += M * x * f,
				s.m_sweep.c.y += M * x * l,
				y = s.m_sweep.c.x + m - i.m_sweep.c.x - p,
				b = s.m_sweep.c.y + g - i.m_sweep.c.y - v
			}
			return this.K1.col1.x = S + x,
			this.K1.col2.x = 0,
			this.K1.col1.y = 0,
			this.K1.col2.y = S + x,
			this.K2.col1.x = T * v * v,
			this.K2.col2.x = -T * p * v,
			this.K2.col1.y = -T * p * v,
			this.K2.col2.y = T * p * p,
			this.K3.col1.x = N * g * g,
			this.K3.col2.x = -N * m * g,
			this.K3.col1.y = -N * m * g,
			this.K3.col2.y = N * m * m,
			this.K.setM(this.K1),
			this.K.addM(this.K2),
			this.K.addM(this.K3),
			this.K.solve(d.dynamics.joints.B2RevoluteJoint.tImpulse, -y, -b),
			f = d.dynamics.joints.B2RevoluteJoint.tImpulse.x,
			l = d.dynamics.joints.B2RevoluteJoint.tImpulse.y,
			i.m_sweep.c.x -= i.m_invMass * f,
			i.m_sweep.c.y -= i.m_invMass * l,
			i.m_sweep.a -= i.m_invI * (p * l - v * f),
			s.m_sweep.c.x += s.m_invMass * f,
			s.m_sweep.c.y += s.m_invMass * l,
			s.m_sweep.a += s.m_invI * (m * l - g * f),
			i.synchronizeTransform(),
			s.synchronizeTransform(),
			u <= d.common.B2Settings.b2_linearSlop && o <= d.common.B2Settings.b2_angularSlop
		},
		solveVelocityConstraints: function (e) {
			var t = this.m_bodyA,
			n = this.m_bodyB,
			r, i, s, o, u, a, f, l = t.m_linearVelocity,
			c = t.m_angularVelocity,
			h = n.m_linearVelocity,
			p = n.m_angularVelocity,
			v = t.m_invMass,
			m = n.m_invMass,
			g = t.m_invI,
			y = n.m_invI;
			if (this.m_enableMotor && this.m_limitState != d.dynamics.joints.B2Joint.e_equalLimits) {
				var b = p - c - this.m_motorSpeed,
				w = this.m_motorMass * -b,
				E = this.m_motorImpulse,
				S = e.dt * this.m_maxMotorTorque;
				this.m_motorImpulse = d.common.math.B2Math.clamp(this.m_motorImpulse + w, -S, S),
				w = this.m_motorImpulse - E,
				c -= g * w,
				p += y * w
			}
			if (this.m_enableLimit && this.m_limitState != d.dynamics.joints.B2Joint.e_inactiveLimit) {
				r = t.m_xf.R,
				o = this.m_localAnchor1.x - t.m_sweep.localCenter.x,
				u = this.m_localAnchor1.y - t.m_sweep.localCenter.y,
				i = r.col1.x * o + r.col2.x * u,
				u = r.col1.y * o + r.col2.y * u,
				o = i,
				r = n.m_xf.R,
				a = this.m_localAnchor2.x - n.m_sweep.localCenter.x,
				f = this.m_localAnchor2.y - n.m_sweep.localCenter.y,
				i = r.col1.x * a + r.col2.x * f,
				f = r.col1.y * a + r.col2.y * f,
				a = i;
				var x = h.x + -p * f - l.x - -c * u,
				T = h.y + p * a - l.y - c * o,
				N = p - c;
				this.m_mass.solve33(this.impulse3, -x, -T, -N),
				this.m_limitState == d.dynamics.joints.B2Joint.e_equalLimits ? this.m_impulse.add(this.impulse3) : this.m_limitState == d.dynamics.joints.B2Joint.e_atLowerLimit ? (s = this.m_impulse.z + this.impulse3.z, s < 0 && (this.m_mass.solve22(this.reduced, -x, -T), this.impulse3.x = this.reduced.x, this.impulse3.y = this.reduced.y, this.impulse3.z = -this.m_impulse.z, this.m_impulse.x += this.reduced.x, this.m_impulse.y += this.reduced.y, this.m_impulse.z = 0)) : this.m_limitState == d.dynamics.joints.B2Joint.e_atUpperLimit && (s = this.m_impulse.z + this.impulse3.z, s > 0 && (this.m_mass.solve22(this.reduced, -x, -T), this.impulse3.x = this.reduced.x, this.impulse3.y = this.reduced.y, this.impulse3.z = -this.m_impulse.z, this.m_impulse.x += this.reduced.x, this.m_impulse.y += this.reduced.y, this.m_impulse.z = 0)),
				l.x -= v * this.impulse3.x,
				l.y -= v * this.impulse3.y,
				c -= g * (o * this.impulse3.y - u * this.impulse3.x + this.impulse3.z),
				h.x += m * this.impulse3.x,
				h.y += m * this.impulse3.y,
				p += y * (a * this.impulse3.y - f * this.impulse3.x + this.impulse3.z)
			} else {
				r = t.m_xf.R,
				o = this.m_localAnchor1.x - t.m_sweep.localCenter.x,
				u = this.m_localAnchor1.y - t.m_sweep.localCenter.y,
				i = r.col1.x * o + r.col2.x * u,
				u = r.col1.y * o + r.col2.y * u,
				o = i,
				r = n.m_xf.R,
				a = this.m_localAnchor2.x - n.m_sweep.localCenter.x,
				f = this.m_localAnchor2.y - n.m_sweep.localCenter.y,
				i = r.col1.x * a + r.col2.x * f,
				f = r.col1.y * a + r.col2.y * f,
				a = i;
				var C = h.x + -p * f - l.x - -c * u,
				k = h.y + p * a - l.y - c * o;
				this.m_mass.solve22(this.impulse2, -C, -k),
				this.m_impulse.x += this.impulse2.x,
				this.m_impulse.y += this.impulse2.y,
				l.x -= v * this.impulse2.x,
				l.y -= v * this.impulse2.y,
				c -= g * (o * this.impulse2.y - u * this.impulse2.x),
				h.x += m * this.impulse2.x,
				h.y += m * this.impulse2.y,
				p += y * (a * this.impulse2.y - f * this.impulse2.x)
			}
			t.m_linearVelocity.setV(l),
			t.m_angularVelocity = c,
			n.m_linearVelocity.setV(h),
			n.m_angularVelocity = p
		},
		initVelocityConstraints: function (e) {
			var t = this.m_bodyA,
			n = this.m_bodyB,
			r, i;
			this.m_enableMotor || this.m_enableLimit,
			r = t.m_xf.R;
			var s = this.m_localAnchor1.x - t.m_sweep.localCenter.x,
			o = this.m_localAnchor1.y - t.m_sweep.localCenter.y;
			i = r.col1.x * s + r.col2.x * o,
			o = r.col1.y * s + r.col2.y * o,
			s = i,
			r = n.m_xf.R;
			var u = this.m_localAnchor2.x - n.m_sweep.localCenter.x,
			a = this.m_localAnchor2.y - n.m_sweep.localCenter.y;
			i = r.col1.x * u + r.col2.x * a,
			a = r.col1.y * u + r.col2.y * a,
			u = i;
			var f = t.m_invMass,
			l = n.m_invMass,
			c = t.m_invI,
			h = n.m_invI;
			this.m_mass.col1.x = f + l + o * o * c + a * a * h,
			this.m_mass.col2.x = -o * s * c - a * u * h,
			this.m_mass.col3.x = -o * c - a * h,
			this.m_mass.col1.y = this.m_mass.col2.x,
			this.m_mass.col2.y = f + l + s * s * c + u * u * h,
			this.m_mass.col3.y = s * c + u * h,
			this.m_mass.col1.z = this.m_mass.col3.x,
			this.m_mass.col2.z = this.m_mass.col3.y,
			this.m_mass.col3.z = c + h,
			this.m_motorMass = 1 / (c + h),
			this.m_enableMotor == 0 && (this.m_motorImpulse = 0);
			if (this.m_enableLimit) {
				var p = n.m_sweep.a - t.m_sweep.a - this.m_referenceAngle;
				d.common.math.B2Math.abs(this.m_upperAngle - this.m_lowerAngle) < 2 * d.common.B2Settings.b2_angularSlop ? this.m_limitState = d.dynamics.joints.B2Joint.e_equalLimits: p <= this.m_lowerAngle ? (this.m_limitState != d.dynamics.joints.B2Joint.e_atLowerLimit && (this.m_impulse.z = 0), this.m_limitState = d.dynamics.joints.B2Joint.e_atLowerLimit) : p >= this.m_upperAngle ? (this.m_limitState != d.dynamics.joints.B2Joint.e_atUpperLimit && (this.m_impulse.z = 0), this.m_limitState = d.dynamics.joints.B2Joint.e_atUpperLimit) : (this.m_limitState = d.dynamics.joints.B2Joint.e_inactiveLimit, this.m_impulse.z = 0)
			} else this.m_limitState = d.dynamics.joints.B2Joint.e_inactiveLimit;
			if (e.warmStarting) {
				this.m_impulse.x *= e.dtRatio,
				this.m_impulse.y *= e.dtRatio,
				this.m_motorImpulse *= e.dtRatio;
				var v = this.m_impulse.x,
				m = this.m_impulse.y;
				t.m_linearVelocity.x -= f * v,
				t.m_linearVelocity.y -= f * m,
				t.m_angularVelocity -= c * (s * m - o * v + this.m_motorImpulse + this.m_impulse.z),
				n.m_linearVelocity.x += l * v,
				n.m_linearVelocity.y += l * m,
				n.m_angularVelocity += h * (u * m - a * v + this.m_motorImpulse + this.m_impulse.z)
			} else this.m_impulse.setZero(),
			this.m_motorImpulse = 0
		},
		getMotorTorque: function () {
			return this.m_maxMotorTorque
		},
		setMaxMotorTorque: function (e) {
			this.m_maxMotorTorque = e
		},
		getMotorSpeed: function () {
			return this.m_motorSpeed
		},
		setMotorSpeed: function (e) {
			this.m_bodyA.setAwake(!0),
			this.m_bodyB.setAwake(!0),
			this.m_motorSpeed = e
		},
		enableMotor: function (e) {
			this.m_enableMotor = e
		},
		isMotorEnabled: function () {
			return this.m_bodyA.setAwake(!0),
			this.m_bodyB.setAwake(!0),
			this.m_enableMotor
		},
		setLimits: function (e, t) {
			this.m_lowerAngle = e,
			this.m_upperAngle = t
		},
		getUpperLimit: function () {
			return this.m_upperAngle
		},
		getLowerLimit: function () {
			return this.m_lowerAngle
		},
		enableLimit: function (e) {
			this.m_enableLimit = e
		},
		isLimitEnabled: function () {
			return this.m_enableLimit
		},
		getJointSpeed: function () {
			return this.m_bodyB.m_angularVelocity - this.m_bodyA.m_angularVelocity
		},
		getJointAngle: function () {
			return this.m_bodyB.m_sweep.a - this.m_bodyA.m_sweep.a - this.m_referenceAngle
		},
		getReactionTorque: function (e) {
			return e * this.m_impulse.z
		},
		getReactionForce: function (e) {
			return new d.common.math.B2Vec2(e * this.m_impulse.x, e * this.m_impulse.y)
		},
		getAnchorB: function () {
			return this.m_bodyB.getWorldPoint(this.m_localAnchor2)
		},
		getAnchorA: function () {
			return this.m_bodyA.getWorldPoint(this.m_localAnchor1)
		},
		__class__: d.dynamics.joints.B2RevoluteJoint
	}),
	d.dynamics.joints.B2RevoluteJointDef = function () {
		d.dynamics.joints.B2JointDef.call(this),
		this.localAnchorA = new d.common.math.B2Vec2,
		this.localAnchorB = new d.common.math.B2Vec2,
		this.type = d.dynamics.joints.B2Joint.e_revoluteJoint,
		this.localAnchorA.set(0, 0),
		this.localAnchorB.set(0, 0),
		this.referenceAngle = 0,
		this.lowerAngle = 0,
		this.upperAngle = 0,
		this.maxMotorTorque = 0,
		this.motorSpeed = 0,
		this.enableLimit = !1,
		this.enableMotor = !1
	},
	d.dynamics.joints.B2RevoluteJointDef.__name__ = !0,
	d.dynamics.joints.B2RevoluteJointDef.__super__ = d.dynamics.joints.B2JointDef,
	d.dynamics.joints.B2RevoluteJointDef.prototype = t(d.dynamics.joints.B2JointDef.prototype, {
		initialize: function (e, t, n) {
			this.bodyA = e,
			this.bodyB = t,
			this.localAnchorA = this.bodyA.getLocalPoint(n),
			this.localAnchorB = this.bodyB.getLocalPoint(n),
			this.referenceAngle = this.bodyB.getAngle() - this.bodyA.getAngle()
		},
		__class__: d.dynamics.joints.B2RevoluteJointDef
	}),
	d.dynamics.joints.B2WeldJoint = function (e) {
		d.dynamics.joints.B2Joint.call(this, e),
		this.m_localAnchorA = new d.common.math.B2Vec2,
		this.m_localAnchorB = new d.common.math.B2Vec2,
		this.m_impulse = new d.common.math.B2Vec3,
		this.m_mass = new d.common.math.B2Mat33,
		this.m_localAnchorA.setV(e.localAnchorA),
		this.m_localAnchorB.setV(e.localAnchorB),
		this.m_referenceAngle = e.referenceAngle,
		this.m_impulse.setZero(),
		this.m_mass = new d.common.math.B2Mat33
	},
	d.dynamics.joints.B2WeldJoint.__name__ = !0,
	d.dynamics.joints.B2WeldJoint.__super__ = d.dynamics.joints.B2Joint,
	d.dynamics.joints.B2WeldJoint.prototype = t(d.dynamics.joints.B2Joint.prototype, {
		solvePositionConstraints: function (e) {
			var t, n, r = this.m_bodyA,
			i = this.m_bodyB;
			t = r.m_xf.R;
			var s = this.m_localAnchorA.x - r.m_sweep.localCenter.x,
			o = this.m_localAnchorA.y - r.m_sweep.localCenter.y;
			n = t.col1.x * s + t.col2.x * o,
			o = t.col1.y * s + t.col2.y * o,
			s = n,
			t = i.m_xf.R;
			var u = this.m_localAnchorB.x - i.m_sweep.localCenter.x,
			a = this.m_localAnchorB.y - i.m_sweep.localCenter.y;
			n = t.col1.x * u + t.col2.x * a,
			a = t.col1.y * u + t.col2.y * a,
			u = n;
			var f = r.m_invMass,
			l = i.m_invMass,
			c = r.m_invI,
			h = i.m_invI,
			p = i.m_sweep.c.x + u - r.m_sweep.c.x - s,
			v = i.m_sweep.c.y + a - r.m_sweep.c.y - o,
			m = i.m_sweep.a - r.m_sweep.a - this.m_referenceAngle,
			g = 10 * d.common.B2Settings.b2_linearSlop,
			y = Math.sqrt(p * p + v * v),
			b = d.common.math.B2Math.abs(m);
			y > g && (c *= 1, h *= 1),
			this.m_mass.col1.x = f + l + o * o * c + a * a * h,
			this.m_mass.col2.x = -o * s * c - a * u * h,
			this.m_mass.col3.x = -o * c - a * h,
			this.m_mass.col1.y = this.m_mass.col2.x,
			this.m_mass.col2.y = f + l + s * s * c + u * u * h,
			this.m_mass.col3.y = s * c + u * h,
			this.m_mass.col1.z = this.m_mass.col3.x,
			this.m_mass.col2.z = this.m_mass.col3.y,
			this.m_mass.col3.z = c + h;
			var w = new d.common.math.B2Vec3;
			return this.m_mass.solve33(w, -p, -v, -m),
			r.m_sweep.c.x -= f * w.x,
			r.m_sweep.c.y -= f * w.y,
			r.m_sweep.a -= c * (s * w.y - o * w.x + w.z),
			i.m_sweep.c.x += l * w.x,
			i.m_sweep.c.y += l * w.y,
			i.m_sweep.a += h * (u * w.y - a * w.x + w.z),
			r.synchronizeTransform(),
			i.synchronizeTransform(),
			y <= d.common.B2Settings.b2_linearSlop && b <= d.common.B2Settings.b2_angularSlop
		},
		solveVelocityConstraints: function (e) {
			var t, n, r = this.m_bodyA,
			i = this.m_bodyB,
			s = r.m_linearVelocity,
			o = r.m_angularVelocity,
			u = i.m_linearVelocity,
			a = i.m_angularVelocity,
			f = r.m_invMass,
			l = i.m_invMass,
			c = r.m_invI,
			h = i.m_invI;
			t = r.m_xf.R;
			var p = this.m_localAnchorA.x - r.m_sweep.localCenter.x,
			v = this.m_localAnchorA.y - r.m_sweep.localCenter.y;
			n = t.col1.x * p + t.col2.x * v,
			v = t.col1.y * p + t.col2.y * v,
			p = n,
			t = i.m_xf.R;
			var m = this.m_localAnchorB.x - i.m_sweep.localCenter.x,
			g = this.m_localAnchorB.y - i.m_sweep.localCenter.y;
			n = t.col1.x * m + t.col2.x * g,
			g = t.col1.y * m + t.col2.y * g,
			m = n;
			var y = u.x - a * g - s.x + o * v,
			b = u.y + a * m - s.y - o * p,
			w = a - o,
			E = new d.common.math.B2Vec3;
			this.m_mass.solve33(E, -y, -b, -w),
			this.m_impulse.add(E),
			s.x -= f * E.x,
			s.y -= f * E.y,
			o -= c * (p * E.y - v * E.x + E.z),
			u.x += l * E.x,
			u.y += l * E.y,
			a += h * (m * E.y - g * E.x + E.z),
			r.m_angularVelocity = o,
			i.m_angularVelocity = a
		},
		initVelocityConstraints: function (e) {
			var t, n, r = this.m_bodyA,
			i = this.m_bodyB;
			t = r.m_xf.R;
			var s = this.m_localAnchorA.x - r.m_sweep.localCenter.x,
			o = this.m_localAnchorA.y - r.m_sweep.localCenter.y;
			n = t.col1.x * s + t.col2.x * o,
			o = t.col1.y * s + t.col2.y * o,
			s = n,
			t = i.m_xf.R;
			var u = this.m_localAnchorB.x - i.m_sweep.localCenter.x,
			a = this.m_localAnchorB.y - i.m_sweep.localCenter.y;
			n = t.col1.x * u + t.col2.x * a,
			a = t.col1.y * u + t.col2.y * a,
			u = n;
			var f = r.m_invMass,
			l = i.m_invMass,
			c = r.m_invI,
			h = i.m_invI;
			this.m_mass.col1.x = f + l + o * o * c + a * a * h,
			this.m_mass.col2.x = -o * s * c - a * u * h,
			this.m_mass.col3.x = -o * c - a * h,
			this.m_mass.col1.y = this.m_mass.col2.x,
			this.m_mass.col2.y = f + l + s * s * c + u * u * h,
			this.m_mass.col3.y = s * c + u * h,
			this.m_mass.col1.z = this.m_mass.col3.x,
			this.m_mass.col2.z = this.m_mass.col3.y,
			this.m_mass.col3.z = c + h,
			e.warmStarting ? (this.m_impulse.x *= e.dtRatio, this.m_impulse.y *= e.dtRatio, this.m_impulse.z *= e.dtRatio, r.m_linearVelocity.x -= f * this.m_impulse.x, r.m_linearVelocity.y -= f * this.m_impulse.y, r.m_angularVelocity -= c * (s * this.m_impulse.y - o * this.m_impulse.x + this.m_impulse.z), i.m_linearVelocity.x += l * this.m_impulse.x, i.m_linearVelocity.y += l * this.m_impulse.y, i.m_angularVelocity += h * (u * this.m_impulse.y - a * this.m_impulse.x + this.m_impulse.z)) : this.m_impulse.setZero()
		},
		getReactionTorque: function (e) {
			return e * this.m_impulse.z
		},
		getReactionForce: function (e) {
			return new d.common.math.B2Vec2(e * this.m_impulse.x, e * this.m_impulse.y)
		},
		getAnchorB: function () {
			return this.m_bodyB.getWorldPoint(this.m_localAnchorB)
		},
		getAnchorA: function () {
			return this.m_bodyA.getWorldPoint(this.m_localAnchorA)
		},
		__class__: d.dynamics.joints.B2WeldJoint
	}),
	d.dynamics.joints.B2WeldJointDef = function () {
		d.dynamics.joints.B2JointDef.call(this),
		this.localAnchorA = new d.common.math.B2Vec2,
		this.localAnchorB = new d.common.math.B2Vec2,
		this.type = d.dynamics.joints.B2Joint.e_weldJoint,
		this.referenceAngle = 0
	},
	d.dynamics.joints.B2WeldJointDef.__name__ = !0,
	d.dynamics.joints.B2WeldJointDef.__super__ = d.dynamics.joints.B2JointDef,
	d.dynamics.joints.B2WeldJointDef.prototype = t(d.dynamics.joints.B2JointDef.prototype, {
		initialize: function (e, t, n) {
			this.bodyA = e,
			this.bodyB = t,
			this.localAnchorA.setV(this.bodyA.getLocalPoint(n)),
			this.localAnchorB.setV(this.bodyB.getLocalPoint(n)),
			this.referenceAngle = this.bodyB.getAngle() - this.bodyA.getAngle()
		},
		__class__: d.dynamics.joints.B2WeldJointDef
	});
	var v = {};
	v.geoquake = {},
	v.geoquake.Angle = function () {},
	v.geoquake.Angle.__name__ = !0,
	v.geoquake.Angle.radian = function (e) {
		return e * .0174532925199433
	},
	v.geoquake.Angle.degree = function (e) {
		return e * 57.29577951308232
	},
	v.geoquake.Angle.wrap = function (e) {
		return e - 6.283185307179586 * Math.floor((e + Math.PI) / 6.283185307179586)
	},
	v.geoquake.BuildDate = function () {},
	v.geoquake.BuildDate.__name__ = !0,
	v.geoquake.MyMath = function () {},
	v.geoquake.MyMath.__name__ = !0,
	v.geoquake.MyMath.random = function (e, t) {
		return e + Math.random() * (t - e)
	},
	v.geoquake.MyMath.randomInt = function (e, t) {
		return e + l.random(t - e + 1)
	},
	v.geoquake.MyMath.clamp = function (e, t, n) {
		if (t > n) {
			var r = t;
			t = n,
			n = r
		}
		return e < t ? t: e > n ? n: e
	},
	v.geoquake.MyMath.linearMap = function (e, t, n, r, i) {
		return r + (e - t) * (i - r) / (n - t)
	},
	v.geoquake.MyMath.linearClampMap = function (e, t, n, r, i) {
		return v.geoquake.MyMath.clamp(v.geoquake.MyMath.linearMap(e, t, n, r, i), r, i)
	},
	v.geoquake.MyMath.lerp = function (e, t, n) {
		return e + (t - e) * n
	},
	v.geoquake.MyMath.inRange = function (e, t, n) {
		return t <= e && e <= n
	},
	v.geoquake.MyMath.wrap = function (e, t) {
		return e > 0 ? e % t: t + e % t
	},
	v.geoquake.MyMath.absoluteSub = function (e, t) {
		return e > 0 ? (e -= t, e < 0 && (e = 0)) : (e += t, e > 0 && (e = 0)),
		e
	},
	v.geoquake.MyMath.sign = function (e) {
		return e > 0 ? 1 : e < 0 ? -1 : 0
	},
	v.geoquake.MyMath.square = function (e) {
		return e * e
	},
	v.geoquake.MyMath.round = function (e, t) {
		var n = Math.pow(10, t);
		return Math.round(e * n) / n
	},
	v.geoquake.MyMath.lengthVector = function (e, t) {
		return Math.sqrt(e * e + t * t)
	},
	v.geoquake.MyMath.ease = function (e, t, n, r) {
		return e + (t - e) * (1 - Math.exp( - n * r))
	},
	v.geoquake.MyMath.valueToTypes = function (e, t) {
		var n = t.length,
		r = !1,
		i = 0;
		while (i < n) {
			var s = i++,
			o = n - 1 - s;
			t[o] = r ? -1 : exp | 0,
			e = e / 10 | 0,
			e == 0 && (r = !0)
		}
	},
	v.geoquake.Stopwatch = function () {
		this.start()
	},
	v.geoquake.Stopwatch.__name__ = !0,
	v.geoquake.Stopwatch.prototype = {
		getTime: function () {
			return g.Timer.stamp() - this.startTime
		},
		start: function () {
			this.startTime = g.Timer.stamp()
		},
		__class__: v.geoquake.Stopwatch
	},
	v.geoquake.geometry = {},
	v.geoquake.geometry.EulerAngles = function (e, t, n) {
		this.heading = e,
		this.pitch = t,
		this.bank = n
	},
	v.geoquake.geometry.EulerAngles.__name__ = !0,
	v.geoquake.geometry.EulerAngles.prototype = {
		toString: function () {
			return "heading: " + this.heading + " pitch: " + this.pitch + " bank: " + this.bank
		},
		__class__: v.geoquake.geometry.EulerAngles
	},
	v.geoquake.geometry.LineSegment2 = function (e, t) {
		this.p1 = e,
		this.p2 = t
	},
	v.geoquake.geometry.LineSegment2.__name__ = !0,
	v.geoquake.geometry.LineSegment2.intersect = function (e, t) {
		return ((e.p1.x - e.p2.x) * (t.p1.y - e.p1.y) + (e.p1.y - e.p2.y) * (e.p1.x - t.p1.x)) * ((e.p1.x - e.p2.x) * (t.p2.y - e.p1.y) + (e.p1.y - e.p2.y) * (e.p1.x - t.p2.x)) < 0 && ((t.p1.x - t.p2.x) * (e.p1.y - t.p1.y) + (t.p1.y - t.p2.y) * (t.p1.x - e.p1.x)) * ((t.p1.x - t.p2.x) * (e.p2.y - t.p1.y) + (t.p1.y - t.p2.y) * (t.p1.x - e.p2.x)) < 0 ? !0 : !1
	},
	v.geoquake.geometry.LineSegment2.prototype = {
		__class__: v.geoquake.geometry.LineSegment2
	},
	v.geoquake.geometry.Matrix3x3 = function () {},
	v.geoquake.geometry.Matrix3x3.__name__ = !0,
	v.geoquake.geometry.Matrix3x3.prototype = {
		multiplyVector: function (e) {
			var t = this.m11 * e.x + this.m12 * e.y + this.m13 * e.z,
			n = this.m21 * e.x + this.m22 * e.y + this.m23 * e.z,
			r = this.m31 * e.x + this.m32 * e.y + this.m33 * e.z;
			return new v.geoquake.geometry.Vector3(t, n, r)
		},
		toString: function () {
			return "[" + this.m11 + " " + this.m12 + " " + this.m13 + "]\n" + "[" + this.m21 + " " + this.m22 + " " + this.m23 + "]\n" + "[" + this.m31 + " " + this.m32 + " " + this.m33 + "]\n"
		},
		setRotationZ: function (e) {
			var t = Math.cos(e),
			n = Math.sin(e);
			this.m11 = t,
			this.m12 = -n,
			this.m13 = 0,
			this.m21 = n,
			this.m22 = t,
			this.m23 = 0,
			this.m31 = 0,
			this.m32 = 0,
			this.m33 = 1
		},
		setRotationY: function (e) {
			var t = Math.cos(e),
			n = Math.sin(e);
			this.m11 = t,
			this.m12 = 0,
			this.m13 = n,
			this.m21 = 0,
			this.m22 = 1,
			this.m23 = 0,
			this.m31 = -n,
			this.m32 = 0,
			this.m33 = t
		},
		setRotationX: function (e) {
			var t = Math.cos(e),
			n = Math.sin(e);
			this.m11 = 1,
			this.m12 = 0,
			this.m13 = 0,
			this.m21 = 0,
			this.m22 = t,
			this.m23 = -n,
			this.m31 = 0,
			this.m32 = n,
			this.m33 = t
		},
		setIdentity: function () {
			this.m11 = 1,
			this.m12 = 0,
			this.m13 = 0,
			this.m21 = 0,
			this.m22 = 1,
			this.m23 = 0,
			this.m31 = 0,
			this.m32 = 0,
			this.m33 = 1
		},
		set: function (e, t, n, r, i, s, o, u, a) {
			this.m11 = e,
			this.m12 = t,
			this.m13 = n,
			this.m21 = r,
			this.m22 = i,
			this.m23 = s,
			this.m31 = o,
			this.m32 = u,
			this.m33 = a
		},
		__class__: v.geoquake.geometry.Matrix3x3
	},
	v.geoquake.geometry.Polygon2 = function (e) {
		e != null ? this.vertices = e: this.vertices = []
	},
	v.geoquake.geometry.Polygon2.__name__ = !0,
	v.geoquake.geometry.Polygon2.prototype = {
		contains: function (e, t) {
			if (this.boundingBox != null && !this.boundingBox.contains(e, t)) return ! 1;
			if (this.vertices.length <= 2) return ! 1;
			var n = 0,
			r = this.vertices[0],
			i = e <= r.x,
			s = t <= r.y,
			o = 1,
			u = this.vertices.length + 1;
			while (o < u) {
				var a = o++,
				f = this.vertices[a % this.vertices.length],
				l = e <= f.x,
				c = t <= f.y;
				s != c && (i == l ? i && (n += s ? -1 : 1) : e <= r.x + (f.x - r.x) * (t - r.y) / (f.y - r.y) && (n += s ? -1 : 1)),
				r = f,
				i = l,
				s = c
			}
			return n != 0
		},
		computeBoundingBox: function () {
			this.boundingBox = v.geoquake.geometry.Rect.createBoundingBox();
			var e = 0,
			t = this.vertices;
			while (e < t.length) {
				var n = t[e]; ++e,
				this.boundingBox.extend(n.x, n.y)
			}
		},
		scale: function (e, t) {
			var n = 0,
			r = this.vertices;
			while (n < r.length) {
				var i = r[n]; ++n,
				i.x *= e,
				i.y *= t
			}
		},
		translate: function (e, t) {
			var n = 0,
			r = this.vertices;
			while (n < r.length) {
				var i = r[n]; ++n,
				i.x += e,
				i.y += t
			}
		},
		addVertex: function (e, t) {
			this.vertices.push(new v.geoquake.geometry.Vector2(e, t))
		},
		__class__: v.geoquake.geometry.Polygon2
	},
	v.geoquake.geometry.Quaternion = function (e, t, n, r) {
		this.x = e,
		this.y = t,
		this.z = n,
		this.w = r
	},
	v.geoquake.geometry.Quaternion.__name__ = !0,
	v.geoquake.geometry.Quaternion.euler = function (e, t, n) {
		var r = v.geoquake.geometry.Quaternion.angleAxis(e, new v.geoquake.geometry.Vector3(0, 0, 1)),
		i = v.geoquake.geometry.Quaternion.angleAxis(t, new v.geoquake.geometry.Vector3(1, 0, 0)),
		s = v.geoquake.geometry.Quaternion.angleAxis(n, new v.geoquake.geometry.Vector3(0, 1, 0));
		return s.mul(i).mul(r)
	},
	v.geoquake.geometry.Quaternion.angleAxis = function (e, t) {
		var n = e / 2,
		r = Math.sin(n);
		return new v.geoquake.geometry.Quaternion(r * t.x, r * t.y, r * t.z, Math.cos(n))
	},
	v.geoquake.geometry.Quaternion.prototype = {
		getAngle: function () {
			return Math.acos(this.w) * 2
		},
		get_eulerAngles: function () {
			var e, t, n, r = -2 * (this.z * this.y - this.w * this.x);
			return Math.abs(r) > .998 ? (t = Math.PI / 2 * r, e = Math.atan2( - this.x * this.y + this.w * this.z, .5 - this.z * this.z - this.y * this.y), n = 0) : (t = Math.asin(r), e = Math.atan2(this.x * this.y + this.w * this.z, .5 - this.x * this.x - this.z * this.z), n = Math.atan2(this.x * this.z + this.w * this.y, .5 - this.x * this.x - this.y * this.y)),
			new v.geoquake.geometry.EulerAngles(e, t, n)
		},
		toString: function () {
			return "(" + this.x + ", " + this.y + ", " + this.z + ", " + this.w + ")"
		},
		inverse: function () {
			var e = this.conjugate();
			return e.div(this.norm()),
			e
		},
		conjugate: function () {
			return new v.geoquake.geometry.Quaternion( - this.x, -this.y, -this.z, this.w)
		},
		norm: function () {
			return Math.sqrt(this.x * this.x + this.y * this.y + this.z * this.z + this.w * this.w)
		},
		div: function (e) {
			this.x /= e,
			this.y /= e,
			this.z /= e,
			this.w /= e
		},
		mul: function (e) {
			var t = this.w * e.x + this.x * e.w + this.y * e.z - this.z * e.y,
			n = this.w * e.y + this.y * e.w + this.z * e.x - this.x * e.z,
			r = this.w * e.z + this.z * e.w + this.x * e.y - this.y * e.x,
			i = this.w * e.w - this.x * e.x - this.y * e.y - this.z * e.z;
			return new v.geoquake.geometry.Quaternion(t, n, r, i)
		},
		copy: function (e) {
			this.x = e.x,
			this.y = e.y,
			this.z = e.z,
			this.w = e.w
		},
		__class__: v.geoquake.geometry.Quaternion
	},
	v.geoquake.geometry.Rect = function (e, t, n, r) {
		r == null && (r = 0),
		n == null && (n = 0),
		t == null && (t = 0),
		e == null && (e = 0),
		this.x0 = e,
		this.y0 = t,
		this.x1 = n,
		this.y1 = r
	},
	v.geoquake.geometry.Rect.__name__ = !0,
	v.geoquake.geometry.Rect.createSquare = function (e, t, n) {
		return new v.geoquake.geometry.Rect(e - n, t - n, e + n, t + n)
	},
	v.geoquake.geometry.Rect.createBoundingBox = function () {
		return new v.geoquake.geometry.Rect(Math.POSITIVE_INFINITY, Math.POSITIVE_INFINITY, Math.NEGATIVE_INFINITY, Math.NEGATIVE_INFINITY)
	},
	v.geoquake.geometry.Rect.prototype = {
		toString: function () {
			return "(" + this.x0 + ", " + this.y0 + ", " + this.x1 + ", " + this.y1 + ")"
		},
		hitTest: function (e) {
			return e.x1 < this.x0 || e.x0 > this.x1 ? !1 : e.y1 < this.y0 || e.y0 > this.y1 ? !1 : !0
		},
		extend: function (e, t) {
			this.x0 > e && (this.x0 = e),
			this.x1 < e && (this.x1 = e),
			this.y0 > t && (this.y0 = t),
			this.y1 < t && (this.y1 = t)
		},
		contains: function (e, t) {
			return this.x0 <= e && e < this.x1 && this.y0 <= t && t < this.y1
		},
		__class__: v.geoquake.geometry.Rect
	},
	v.geoquake.geometry.Vector2 = function (e, t) {
		this.x = e,
		this.y = t
	},
	v.geoquake.geometry.Vector2.__name__ = !0,
	v.geoquake.geometry.Vector2.distanceSquared = function (e, t) {
		var n = e.x - t.x,
		r = e.y - t.y;
		return n * n + r * r
	},
	v.geoquake.geometry.Vector2.distance = function (e, t) {
		var n = e.x - t.x,
		r = e.y - t.y;
		return Math.sqrt(n * n + r * r)
	},
	v.geoquake.geometry.Vector2.dot = function (e, t) {
		return e.x * t.x + e.y * t.y
	},
	v.geoquake.geometry.Vector2.lerp = function (e, t, n) {
		var r = e.x + (t.x - e.x) * n,
		i = e.y + (t.y - e.y) * n;
		return new v.geoquake.geometry.Vector2(r, i)
	},
	v.geoquake.geometry.Vector2.prototype = {
		scale: function (e) {
			this.x *= e.x,
			this.y *= e.y
		},
		div: function (e) {
			return new v.geoquake.geometry.Vector2(this.x / e, this.y / e)
		},
		mul: function (e) {
			return new v.geoquake.geometry.Vector2(this.x * e, this.y * e)
		},
		rotate: function (e) {
			var t = Math.cos(e),
			n = Math.sin(e),
			r = this.x * t - this.y * n,
			i = this.x * n + this.y * t;
			this.x = r,
			this.y = i
		},
		toString: function () {
			return "(" + this.x + ", " + this.y + ")"
		},
		clone: function () {
			return new v.geoquake.geometry.Vector2(this.x, this.y)
		},
		copy: function (e) {
			this.x = e.x,
			this.y = e.y
		},
		set: function (e, t) {
			this.x = e,
			this.y = t
		},
		__class__: v.geoquake.geometry.Vector2
	},
	v.geoquake.geometry.Vector3 = function (e, t, n) {
		n == null && (n = 0),
		t == null && (t = 0),
		e == null && (e = 0),
		this.x = e,
		this.y = t,
		this.z = n
	},
	v.geoquake.geometry.Vector3.__name__ = !0,
	v.geoquake.geometry.Vector3.zero = function () {
		return new v.geoquake.geometry.Vector3(0, 0, 0)
	},
	v.geoquake.geometry.Vector3.axisX = function () {
		return new v.geoquake.geometry.Vector3(1, 0, 0)
	},
	v.geoquake.geometry.Vector3.axisY = function () {
		return new v.geoquake.geometry.Vector3(0, 1, 0)
	},
	v.geoquake.geometry.Vector3.axisZ = function () {
		return new v.geoquake.geometry.Vector3(0, 0, 1)
	},
	v.geoquake.geometry.Vector3.subVV = function (e, t) {
		return new v.geoquake.geometry.Vector3(e.x - t.x, e.y - t.y, e.z - t.z)
	},
	v.geoquake.geometry.Vector3.distanceSquared = function (e, t) {
		var n = e.x - t.x,
		r = e.y - t.y,
		i = e.z - t.z;
		return n * n + r * r + i * i
	},
	v.geoquake.geometry.Vector3.distance = function (e, t) {
		var n = e.x - t.x,
		r = e.y - t.y,
		i = e.z - t.z;
		return Math.sqrt(n * n + r * r + i * i)
	},
	v.geoquake.geometry.Vector3.dot = function (e, t) {
		return e.x * t.x + e.y * t.y + e.z * t.z
	},
	v.geoquake.geometry.Vector3.cross = function (e, t) {
		var n = e.y * t.z - e.z * t.y,
		r = e.z * t.x - e.x * t.z,
		i = e.x * t.y - e.y * t.x;
		return new v.geoquake.geometry.Vector3(n, r, i)
	},
	v.geoquake.geometry.Vector3.project = function (e, t) {
		return e.mul(v.geoquake.geometry.Vector3.dot(e, t))
	},
	v.geoquake.geometry.Vector3.lerp = function (e, t, n) {
		var r = e.x + (t.x - e.x) * n,
		i = e.y + (t.y - e.y) * n,
		s = e.z + (t.z - e.z) * n;
		return new v.geoquake.geometry.Vector3(r, i, s)
	},
	v.geoquake.geometry.Vector3.prototype = {
		get_sqrMagnitude: function () {
			return this.x * this.x + this.y * this.y + this.z * this.z
		},
		get_magnitude: function () {
			return Math.sqrt(this.x * this.x + this.y * this.y + this.z * this.z)
		},
		get_normalized: function () {
			var e = this.get_magnitude();
			return new v.geoquake.geometry.Vector3(this.x / e, this.y / e, this.z / e)
		},
		normalize: function () {
			var e = this.get_magnitude();
			this.x /= e,
			this.y /= e,
			this.z /= e
		},
		scale: function (e) {
			this.x *= e.x,
			this.y *= e.y,
			this.z *= e.z
		},
		div: function (e) {
			return new v.geoquake.geometry.Vector3(this.x / e, this.y / e, this.z / e)
		},
		mul: function (e) {
			return new v.geoquake.geometry.Vector3(this.x * e, this.y * e, this.z * e)
		},
		rotateZ: function (e) {
			var t = Math.cos(e),
			n = Math.sin(e),
			r = this.x * t - this.y * n,
			i = this.x * n + this.y * t;
			this.x = r,
			this.y = i
		},
		rotate: function (e) {
			var t = new v.geoquake.geometry.Quaternion(this.x, this.y, this.z, 0),
			n = e.inverse(),
			r = e.mul(t).mul(n);
			this.x = r.x,
			this.y = r.y,
			this.z = r.z
		},
		toString: function () {
			return "(" + this.x + ", " + this.y + ", " + this.z + ")"
		},
		multiply: function (e) {
			var t = e.m11 * this.x + e.m12 * this.y + e.m13 * this.z,
			n = e.m21 * this.x + e.m22 * this.y + e.m23 * this.z,
			r = e.m31 * this.x + e.m32 * this.y + e.m33 * this.z;
			this.x = t,
			this.y = n,
			this.z = r
		},
		clone: function () {
			return new v.geoquake.geometry.Vector3(this.x, this.y, this.z)
		},
		copy: function (e) {
			this.x = e.x,
			this.y = e.y,
			this.z = e.z
		},
		set: function (e, t, n) {
			this.x = e,
			this.y = t,
			this.z = n
		},
		__class__: v.geoquake.geometry.Vector3
	},
	v.geoquake.js = {},
	v.geoquake.js.Debug = function () {},
	v.geoquake.js.Debug.__name__ = !0,
	v.geoquake.js.Debug.clear = function () {
		var e = y.Browser.document.getElementById("debug_log");
		e != null && (e.innerHTML = "", v.geoquake.js.Debug.logLine = 0)
	},
	v.geoquake.js.Debug.log = function (e) {
		var t = y.Browser.document.getElementById("debug_log");
		t != null && (t.innerHTML += "<div>" + l.string(e) + "</div>", ++v.geoquake.js.Debug.logLine >= v.geoquake.js.Debug.logHeight && t.removeChild(t.firstChild));
		var n = y.Browser.window;
		n.console && n.console.log(e)
	},
	v.geoquake.js.Debug.setLogHeight = function (e) {
		v.geoquake.js.Debug.logHeight = e
	},
	v.geoquake.js.Debug.watch = function (e, t) {
		var n = t;
		v.geoquake.js.Debug.watchParams.set(e, n)
	},
	v.geoquake.js.Debug.update = function () {
		var e = "",
		t = v.geoquake.js.Debug.watchParams.keys();
		while (t.hasNext()) {
			var n = t.next();
			e += n + ": " + v.geoquake.js.Debug.watchParams.get(n) + "<br>"
		}
		var r = y.Browser.document.getElementById("debug_watch");
		r != null && (r.innerHTML = e)
	},
	v.geoquake.js.Debug.drawPolygon = function (e, t) {
		e.beginPath(),
		e.moveTo(t.vertices[0].x, t.vertices[0].y);
		var n = 1,
		r = t.vertices.length;
		while (n < r) {
			var i = n++;
			e.lineTo(t.vertices[i].x, t.vertices[i].y)
		}
		e.closePath()
	},
	v.geoquake.js.Debug.benchmark = function (e, t) {
		var n = new v.geoquake.Stopwatch;
		t(),
		v.geoquake.js.Debug.log(e + ": " + n.getTime() + " s")
	},
	v.geoquake.js.DomainChecker = function () {
		this.privateNetwork = !1,
		this.domains = []
	},
	v.geoquake.js.DomainChecker.__name__ = !0,
	v.geoquake.js.DomainChecker.prototype = {
		check: function () {
			var e = y.Browser.window.location.hostname,
			t = 0,
			n = this.domains.length;
			while (t < n) {
				var r = t++;
				if (e == this.domains[r]) return ! 0
			}
			if (this.privateNetwork) if (e == "localhost" || e.indexOf("10.") == 0 || e.indexOf("172.") == 0 || e.indexOf("192.168.") == 0) return ! 0;
			return ! 1
		},
		allowPrivateNetwork: function () {
			this.privateNetwork = !0
		},
		registerDomain: function (e) {
			this.domains.push(e)
		},
		__class__: v.geoquake.js.DomainChecker
	},
	v.geoquake.js.GamePad = function () {},
	v.geoquake.js.GamePad.__name__ = !0,
	v.geoquake.js.GamePad.getGamepads = function () {
		var e = y.Browser.window.navigator;
		return e.webkitGetGamepads ? e.webkitGetGamepads() : e.webkitGamepads
	},
	v.geoquake.js.GamePad.getAxis = function (e) {
		var t = v.geoquake.js.GamePad.getGamepads();
		return t && t[0] ? t[0].axes[e] : 0
	},
	v.geoquake.js.GamePad.getButton = function (e) {
		var t = v.geoquake.js.GamePad.getGamepads();
		return t && t[0] ? t[0].buttons[e] : 0
	},
	v.geoquake.js.Keyboard = function (e) {
		this.active = !0,
		this.element = e,
		this.keyState = [],
		this.keyCount = [],
		this.preventKeys = [32, 33, 34, 35, 36, 37, 38, 39, 40];
		var t = 1;
		while (t < 100) {
			var n = t++;
			this.keyCount[n] = 0
		}
		e.addEventListener ? (e.addEventListener("keydown", E(this, this.onKeyDown), !0), e.addEventListener("keyup", E(this, this.onKeyUp), !0)) : (e.attachEvent("onkeydown", E(this, this.onKeyDown)), e.attachEvent("onkeyup", E(this, this.onKeyUp)))
	},
	v.geoquake.js.Keyboard.__name__ = !0,
	v.geoquake.js.Keyboard.prototype = {
		isKeyPressed: function () {
			return this.keyState[32] || this.keyState[17] ? !0 : !1
		},
		getStickY: function () {
			return this.keyState[38] ? 1 : this.keyState[40] ? -1 : 0
		},
		getStickX: function () {
			return this.keyState[39] ? 1 : this.keyState[37] ? -1 : 0
		},
		isPressed: function (e) {
			return this.keyCount[e] == 1
		},
		isDown: function (e) {
			return this.keyState[e]
		},
		update: function () {
			var e = 1;
			while (e < 100) {
				var t = e++;
				this.keyState[t] ? this.keyCount[t]++:this.keyCount[t] = 0
			}
		},
		onKeyUp: function (e) {
			var t = y.Browser.window,
			n = t.event ? t.event.keyCode: e.keyCode;
			this.keyState[n] = !1;
			var r = 0,
			i = this.preventKeys;
			while (r < i.length) {
				var s = i[r]; ++r,
				n == s && (e.preventDefault ? e.preventDefault() : e.returnValue = !1)
			}
		},
		onKeyDown: function (e) {
			if (!this.active) return;
			var t = y.Browser.window,
			n = t.event ? t.event.keyCode: e.keyCode;
			this.keyState[n] = !0;
			var r = 0,
			i = this.preventKeys;
			while (r < i.length) {
				var s = i[r]; ++r,
				n == s && (e.preventDefault ? e.preventDefault() : e.returnValue = !1)
			}
		},
		__class__: v.geoquake.js.Keyboard
	},
	v.geoquake.js.Platform = function () {},
	v.geoquake.js.Platform.__name__ = !0,
	v.geoquake.js.Platform.isChrome = function () {
		return v.geoquake.js.Platform.userAgentContains("Chrome")
	},
	v.geoquake.js.Platform.isFirefox = function () {
		return v.geoquake.js.Platform.userAgentContains("Firefox")
	},
	v.geoquake.js.Platform.isIE = function () {
		return v.geoquake.js.Platform.userAgentContains("MSIE")
	},
	v.geoquake.js.Platform.isSafari = function () {
		return v.geoquake.js.Platform.userAgentContains("Safari") && v.geoquake.js.Platform.vendorContains("Apple Computer")
	},
	v.geoquake.js.Platform.isiPhone = function () {
		return v.geoquake.js.Platform.userAgentContains("iPhone")
	},
	v.geoquake.js.Platform.isiPad = function () {
		return v.geoquake.js.Platform.userAgentContains("iPad")
	},
	v.geoquake.js.Platform.isiOS = function () {
		return v.geoquake.js.Platform.isiPhone() || v.geoquake.js.Platform.isiPad()
	},
	v.geoquake.js.Platform.isAndroid = function () {
		return v.geoquake.js.Platform.userAgentContains("Android")
	},
	v.geoquake.js.Platform.isWebKit = function () {
		return v.geoquake.js.Platform.userAgentContains("WebKit")
	},
	v.geoquake.js.Platform.isMobile = function () {
		return v.geoquake.js.Platform.isiOS() || v.geoquake.js.Platform.isAndroid()
	},
	v.geoquake.js.Platform.isTouchDevice = function () {
		return v.geoquake.js.Platform.isMobile()
	},
	v.geoquake.js.Platform.isTablet = function () {
		var e = v.geoquake.js.Platform.isAndroid() && !v.geoquake.js.Platform.userAgentContains("mobile"),
		t = v.geoquake.js.Platform.isiPad();
		return e || t
	},
	v.geoquake.js.Platform.getLanguage = function () {
		try {
			return (navigator.browserLanguage || navigator.language || navigator.userLanguage).substr(0, 2)
		} catch(e) {
			return null
		}
	},
	v.geoquake.js.Platform.getUserAgent = function () {
		return y.Browser.window.navigator.userAgent
	},
	v.geoquake.js.Platform.userAgentContains = function (e) {
		return v.geoquake.js.Platform.getUserAgent().indexOf(e) >= 0
	},
	v.geoquake.js.Platform.vendorContains = function (e) {
		var t = y.Browser.window.navigator;
		return t.vendor.indexOf(e) >= 0
	};
	var m = {};
	m.GroundOverlay = function (e, t) {
		this.ge = e,
		this.overlay = e.createGroundOverlay(""),
		this.overlay.setIcon(e.createIcon("")),
		this.overlay.getIcon().setHref(t),
		this.overlay.setLatLonBox(e.createLatLonBox("")),
		e.getFeatures().appendChild(this.overlay)
	},
	m.GroundOverlay.__name__ = !0,
	m.GroundOverlay.prototype = {
		setLatLngRot: function (e, t, n) {
			var r = e.createOffset(0, n),
			i = e.createOffset(1.570796326794897, n),
			s = r.lat - e.lat,
			o = i.lng - e.lng,
			u = e.lat + s,
			a = e.lat - s,
			f = e.lng + o,
			l = e.lng - o;
			t *= 57.29577951308232;
			var c = this.overlay.getLatLonBox();
			c.setBox(u, a, f, l, t)
		},
		__class__: m.GroundOverlay
	},
	m.KMLFetcher = function (e, t) {
		this.ge = e,
		this.url = t
	},
	m.KMLFetcher.__name__ = !0,
	m.KMLFetcher.prototype = {
		onFetched: function () {
			v.geoquake.js.Debug.log("onFetched"),
			this.onLoad != null && this.onLoad()
		},
		load: function (e) {
			v.geoquake.js.Debug.log("load"),
			this.onLoad = e;
			try {
				google.earth.fetchKml(this.ge, this.url, E(this, this.onFetched))
			} catch(t) {
				v.geoquake.js.Debug.log("fetchKml error retry");
				setTimeout(E(this, this.load), 100)
			}
		},
		__class__: m.KMLFetcher
	},
	m.KMLLoader = function (e, t) {
		this.ge = e,
		this.url = t
	},
	m.KMLLoader.__name__ = !0,
	m.KMLLoader.prototype = {
		onFetched: function (e) {
			if (e == null) {
				v.geoquake.js.Debug.log("null"),
				this.onLoad != null && this.onLoad();
				return
			}
			try {
				v.geoquake.js.Debug.log("loaded: " + e.getName()),
				this.kmlFolder = e;
				var t = e.getFeatures().getChildNodes().getLength();
				this.sprites = new g.ds.StringMap,
				this.items = new g.ds.StringMap;
				var n = e.getFeatures().getChildNodes(),
				r = 0;
				while (r < t) {
					var i = r++,
					s = n.item(i),
					o = s.getType(),
					u = s.getName();
					v.geoquake.js.Debug.log(o + ":" + u),
					this.items.set(u, s),
					o == "KmlScreenOverlay"
				}
				this.onLoad != null && this.onLoad()
			} catch(a) {
				v.geoquake.js.Debug.log(a)
			}
		},
		load: function (e) {
			v.geoquake.js.Debug.log("load"),
			this.onLoad = e;
			try {
				google.earth.fetchKml(this.ge, this.url, E(this, this.onFetched))
			} catch(t) {
				v.geoquake.js.Debug.log("fetchKml error retry");
				setTimeout(E(this, this.load), 100)
			}
		},
		__class__: m.KMLLoader
	},
	m.Model = function (e, t) {
		this.loaded = !1,
		this.ge = e,
		this.url = t,
		m.Model.expirationCode != null && (this.url += "?" + m.Model.expirationCode)
	},
	m.Model.__name__ = !0,
	m.Model.prototype = {
		setVisibility: function (e) {
			this.loaded && this.placemark.setVisibility(e)
		},
		setLocation: function (e, t, n, r, i, s) {
			this.loaded && (this.location.setLatLngAlt(e, t, n), this.orientation.set(r * 57.29577951308232, i * 57.29577951308232, s * 57.29577951308232))
		},
		onFetched: function (e) {
			try {
				v.geoquake.js.Debug.log("loaded: " + e.getName()),
				this.placemark = e.getFeatures().getChildNodes().item(1),
				this.geometry = this.placemark.getGeometry(),
				this.geometry.setAltitudeMode(this.ge.ALTITUDE_ABSOLUTE),
				this.location = this.geometry.getLocation(),
				this.orientation = this.geometry.getOrientation(),
				this.ge.getFeatures().appendChild(this.placemark),
				this.loaded = !0
			} catch(t) {
				v.geoquake.js.Debug.log(t)
			}
		},
		load: function () {
			try {
				google.earth.fetchKml(this.ge, this.url, E(this, this.onFetched))
			} catch(e) {
				console.log("fetchKml error retry");
				setTimeout(E(this, this.load), 100)
			}
		},
		__class__: m.Model
	},
	m.Sprite = function (e, t) {
		this.opacityCache = 1,
		this.ge = e,
		this.overlay = e.createScreenOverlay(""),
		this.overlay.setIcon(e.createIcon("")),
		this.overlay.getIcon().setHref(t),
		e.getFeatures().appendChild(this.overlay)
	},
	m.Sprite.__name__ = !0,
	m.Sprite.prototype = {
		setDrawOrder: function (e) {
			this.overlay.setDrawOrder(e)
		},
		setVisibility: function (e) {
			this.overlay.setVisibility(e)
		},
		setOpacity: function (e) {
			this.opacityCache != e && (this.opacityCache > 0 && e == 0 && this.setVisibility(!1), this.opacityCache == 0 && e > 0 && this.setVisibility(!0), this.opacityCache = e, this.overlay.setOpacity(e))
		},
		setSize: function (e, t) {
			this.overlay.getSize().set(e, this.ge.UNITS_PIXELS, t, this.ge.UNITS_PIXELS)
		},
		setRotation: function (e) {
			this.overlay.setRotation(e)
		},
		setPosition: function (e, t, n, r) {
			this.overlay.getScreenXY().set(n, this.ge.UNITS_FRACTION, r, this.ge.UNITS_FRACTION),
			this.overlay.getOverlayXY().set(e, this.ge.UNITS_PIXELS, t, this.ge.UNITS_PIXELS)
		},
		__class__: m.Sprite
	},
	m.SpriteNumber = function (e, t, n, r, i, s, o) {
		this.ge = e,
		this.types = [];
		var u = 0;
		while (u < s) {
			var a = u++;
			this.types.push( - 1)
		}
		this.sprites = [];
		var u = 0;
		while (u < s) {
			var a = u++;
			this.sprites[a] = [];
			var f = 0;
			while (f < 10) {
				var l = f++;
				this.sprites[a][l] = new m.Sprite(e, o + l + ".png"),
				this.sprites[a][l].setPosition(t + r * a, n, 0, 0),
				this.sprites[a][l].setOpacity(.005),
				this.sprites[a][l].setDrawOrder(100)
			}
		}
	},
	m.SpriteNumber.__name__ = !0,
	m.SpriteNumber.prototype = {
		setValue: function (e) {
			v.geoquake.MyMath.valueToTypes(
			e, this.types);
			var t = 0;
			while (t < 3) {
				var n = t++,
				r = 0;
				while (r < 10) {
					var i = r++;
					this.sprites[n][i].setOpacity(i == this.types[n] ? 1 : .005)
				}
			}
		},
		__class__: m.SpriteNumber
	};
	var g = {};
	g.Timer = function () {},
	g.Timer.__name__ = !0,
	g.Timer.stamp = function () {
		return (new Date).getTime() / 1e3
	},
	g.ds = {},
	g.ds.StringMap = function () {
		this.h = {}
	},
	g.ds.StringMap.__name__ = !0,
	g.ds.StringMap.prototype = {
		keys: function () {
			var e = [];
			for (var t in this.h) this.h.hasOwnProperty(t) && e.push(t.substr(1));
			return i.iter(e)
		},
		get: function (e) {
			return this.h["$" + e]
		},
		set: function (e, t) {
			this.h["$" + e] = t
		},
		__class__: g.ds.StringMap
	};
	var y = {};
	y.Boot = function () {},
	y.Boot.__name__ = !0,
	y.Boot.__string_rec = function (e, t) {
		if (e == null) return "null";
		if (t.length >= 5) return "<...>";
		var n = typeof e;
		n == "function" && (e.__name__ || e.__ename__) && (n = "object");
		switch (n) {
		case "object":
			if (e instanceof Array) {
				if (e.__enum__) {
					if (e.length == 2) return e[0];
					var r = e[0] + "(";
					t += "	";
					var i = 2,
					s = e.length;
					while (i < s) {
						var o = i++;
						o != 2 ? r += "," + y.Boot.__string_rec(e[o], t) : r += y.Boot.__string_rec(e[o], t)
					}
					return r + ")"
				}
				var u = e.length,
				o, r = "[";
				t += "	";
				var s = 0;
				while (s < u) {
					var a = s++;
					r += (a > 0 ? ",": "") + y.Boot.__string_rec(e[a], t)
				}
				return r += "]",
				r
			}
			var f;
			try {
				f = e.toString
			} catch(l) {
				return "???"
			}
			if (f != null && f != Object.toString) {
				var c = e.toString();
				if (c != "[object Object]") return c
			}
			var h = null,
			r = "{\n";
			t += "	";
			var p = e.hasOwnProperty != null;
			for (var h in e) {
				if (p && !e.hasOwnProperty(h)) continue;
				if (h == "prototype" || h == "__class__" || h == "__super__" || h == "__interfaces__" || h == "__properties__") continue;
				r.length != 2 && (r += ", \n"),
				r += t + h + " : " + y.Boot.__string_rec(e[h], t)
			}
			return t = t.substring(1),
			r += "\n" + t + "}",
			r;
		case "function":
			return "<function>";
		case "string":
			return e;
		default:
			return String(e)
		}
	},
	y.Boot.__interfLoop = function (e, t) {
		if (e == null) return ! 1;
		if (e == t) return ! 0;
		var n = e.__interfaces__;
		if (n != null) {
			var r = 0,
			i = n.length;
			while (r < i) {
				var s = r++,
				o = n[s];
				if (o == t || y.Boot.__interfLoop(o, t)) return ! 0
			}
		}
		return y.Boot.__interfLoop(e.__super__, t)
	},
	y.Boot.__instanceof = function (e, t) {
		try {
			if (e instanceof t) return t == Array ? e.__enum__ == null: !0;
			if (y.Boot.__interfLoop(e.__class__, t)) return ! 0
		} catch(n) {
			if (t == null) return ! 1
		}
		switch (t) {
		case S:
		case N:
			return e === !0 || e === !1;
		case String:
			return typeof e == "string";
		case x:
			return ! 0;
		default:
			if (e == null) return ! 1;
			if (t == C && e.__name__ != null) return ! 0;
			null;
			if (t == k && e.__ename__ != null) return ! 0;
			return null,
			e.__enum__ == t
		}
	},
	y.Boot.__cast = function (e, t) {
		if (y.Boot.__instanceof(e, t)) return e;
		throw "Cannot cast " + l.string(e) + " to " + l.string(t)
	},
	y.Browser = function () {},
	y.Browser.__name__ = !0;
	var b = {};
	b.Accelerometer = function () {
		this.prevPosition = new v.geoquake.geometry.Vector3(0, 0, 0),
		this.prevVelocity = new v.geoquake.geometry.Vector3(0, 0, 0),
		this.position = new v.geoquake.geometry.Vector3(0, 0, 0),
		this.velocity = new v.geoquake.geometry.Vector3(0, 0, 0),
		this.acceleration = new v.geoquake.geometry.Vector3(0, 0, 0)
	},
	b.Accelerometer.__name__ = !0,
	b.Accelerometer.prototype = {
		getVector: function () {
			return this.acceleration.clone()
		},
		getG: function () {
			return this.acceleration.get_magnitude() / 9.80665
		},
		update: function (e, t) {
			this.prevPosition.copy(this.position),
			this.position.copy(t),
			this.prevVelocity.copy(this.velocity),
			this.velocity = v.geoquake.geometry.Vector3.subVV(this.position, this.prevPosition).div(e),
			this.acceleration = v.geoquake.geometry.Vector3.subVV(this.velocity, this.prevVelocity).div(e)
		},
		__class__: b.Accelerometer
	},
	b.Box2DUtils = function () {},
	b.Box2DUtils.__name__ = !0,
	b.Box2DUtils.toKmh = function (e) {
		return e * 3600 / 1e3
	},
	b.Box2DUtils.toMps = function (e) {
		return e * 1e3 / 3600
	},
	b.Box2DUtils.createBox = function (e, t, n, r, i, s, o, u, a, f) {
		f == null && (f = .2),
		a == null && (a = .5),
		u == null && (u = 1),
		o == null && (o = 0);
		var l = new d.dynamics.B2BodyDef;
		l.type = d.dynamics.B2Body.b2_staticBody,
		l.position.set(t, n);
		var c = new d.dynamics.B2FixtureDef;
		c.density = u,
		c.friction = a,
		c.restitution = f;
		var h = new d.collision.shapes.B2PolygonShape;
		h.setAsBox(r, i),
		c.shape = h;
		var p = e.createBody(l);
		return s != null && p.setPosition(s),
		p.setAngle(o),
		p.createFixture(c),
		p
	},
	b.Box2DUtils.createCircle = function (e, t, n, r, i, s, o) {
		o == null && (o = .2),
		s == null && (s = .5),
		i == null && (i = 1);
		var u = new d.dynamics.B2BodyDef;
		u.type = d.dynamics.B2Body.b2_dynamicBody,
		u.position.set(t, n);
		var a = new d.dynamics.B2FixtureDef;
		a.density = i,
		a.friction = s,
		a.restitution = o;
		var f = new d.collision.shapes.B2CircleShape;
		f.setRadius(r),
		a.shape = f;
		var l = e.createBody(u);
		return l.createFixture(a),
		l
	},
	b.Box2DUtils.createEdge = function (e, t, n, r, i) {
		var s = new d.dynamics.B2BodyDef;
		s.type = d.dynamics.B2Body.b2_staticBody;
		var o = new d.common.math.B2Vec2(t, n),
		u = new d.common.math.B2Vec2(r, i),
		a = new d.collision.shapes.B2PolygonShape;
		a.setAsEdge(o, u);
		var f = e.createBody(s);
		return f.createFixture2(a),
		f
	},
	b.Box2DUtils.createPolygon = function (e, t, n, r, i, s, o, u, a) {
		a == null && (a = 65535),
		u == null && (u = 1),
		o == null && (o = .2),
		s == null && (s = .5),
		i == null && (i = 1),
		r == null && (r = 0);
		if (t.length >= 3) {
			var f = 0,
			l = t.length,
			c = 0;
			while (c < l) {
				var h = c++;
				f += (t[h].x + t[(h + 1) % l].x) * (t[(h + 1) % l].y - t[h].y)
			}
			f < 0 && t.reverse()
		}
		var p = new d.dynamics.B2BodyDef;
		p.type = d.dynamics.B2Body.b2_staticBody,
		n != null && p.position.set(n.x, n.y),
		p.angle = r;
		var v = new d.collision.shapes.B2PolygonShape;
		v.setAsVector(t);
		var m = new d.dynamics.B2FixtureDef;
		m.density = i,
		m.friction = s,
		m.restitution = o,
		m.shape = v,
		m.filter.categoryBits = u,
		m.filter.maskBits = a;
		var g = e.createBody(p);
		return g.createFixture(m),
		g
	},
	b.Box2DUtils.getFrontVector = function (e) {
		return e.getWorldVector(new d.common.math.B2Vec2(0, -1))
	},
	b.Box2DUtils.project = function (e, t) {
		return d.common.math.B2Math.mulFV(d.common.math.B2Math.dot(e, t), e)
	},
	b.GearBox = function () {
		this.gear = 1
	},
	b.GearBox.__name__ = !0,
	b.GearBox.prototype = {
		getCurrentGear: function () {
			return this.gear
		},
		gearDown: function () {
			this.gear = -1
		},
		gearUp: function () {
			this.gear = 1
		},
		getGearRatio: function () {
			var e = this;
			switch (e.gear) {
			case 1:
				return 1;
			case - 1 : return - 1
			}
			return 0
		},
		__class__: b.GearBox
	},
	b.Light = function () {
		this.targetBrightness = 0,
		this.brightness = 0
	},
	b.Light.__name__ = !0,
	b.Light.prototype = {
		update: function (e) {
			this.brightness = v.geoquake.MyMath.ease(this.brightness, this.targetBrightness, 20, e)
		},
		turnOff: function () {
			this.targetBrightness = 0
		},
		turnOn: function () {
			this.targetBrightness = 1
		},
		__class__: b.Light
	},
	b.MagicFormula = function (e, t, n, r) {
		this.b = e,
		this.c = t,
		this.d = n,
		this.e = r
	},
	b.MagicFormula.__name__ = !0,
	b.MagicFormula.prototype = {
		calc: function (e) {
			return this.d * Math.sin(this.c * Math.atan(this.b * (1 - this.e) * e + this.e * Math.atan(this.b * e)))
		},
		__class__: b.MagicFormula
	},
	b.Steering = function (e, t) {
		this.deltaAngle = 0,
		this.lever = 0,
		this.angle = 0,
		this.maxAngle = e * .0174532925199433,
		this.rudderAngleScale = t
	},
	b.Steering.__name__ = !0,
	b.Steering.prototype = {
		getRudderAngle: function () {
			return this.angle / this.rudderAngleScale
		},
		setDeltaAngle: function (e) {
			this.deltaAngle = e
		},
		setLever: function (e) {
			this.lever = e
		},
		update: function (e, t, n) {
			this.deltaAngle = this.lever * 8.72664625997165 * e,
			this.angle += this.deltaAngle;
			var r = v.geoquake.MyMath.linearClampMap(Math.abs(n), 10, 20, 1, 0);
			Math.isNaN(r) && (r = 0);
			var i = v.geoquake.MyMath.linearMap(t, 0, 70, 0, 8);
			this.angle = v.geoquake.MyMath.ease(this.angle, 0, i * r, e),
			this.angle = v.geoquake.MyMath.clamp(this.angle, -this.maxAngle, this.maxAngle)
		},
		__class__: b.Steering
	},
	b.Suspension = function (e, t, n, r, i) {
		i == null && (i = 5e3),
		r == null && (r = 8e4),
		n == null && (n = 375),
		this.velocity = 0,
		this.position = e,
		this.targetPosition = t,
		this.mass = n,
		this.spring = r,
		this.damper = i
	},
	b.Suspension.__name__ = !0,
	b.Suspension.prototype = {
		update: function (e) {
			var t = this.spring * (this.targetPosition - this.position) - this.damper * this.velocity,
			n = t / this.mass;
			this.velocity += n * e,
			this.position += this.velocity * e
		},
		__class__: b.Suspension
	},
	b._TurnSignals = {},
	b._TurnSignals.State = {
		__ename__: !0,
		__constructs__: ["off", "left", "right"]
	},
	b._TurnSignals.State.off = ["off", 0],
	b._TurnSignals.State.off.toString = e,
	b._TurnSignals.State.off.__enum__ = b._TurnSignals.State,
	b._TurnSignals.State.left = ["left", 1],
	b._TurnSignals.State.left.toString = e,
	b._TurnSignals.State.left.__enum__ = b._TurnSignals.State,
	b._TurnSignals.State.right = ["right", 2],
	b._TurnSignals.State.right.toString = e,
	b._TurnSignals.State.right.__enum__ = b._TurnSignals.State,
	b.TurnSignals = function () {
		this.steeringAngle = 0,
		this.notchCount = 0,
		this.notchArea = 0,
		this.blinkCircuit = !1,
		this.intervalTime = 300,
		this.enabledHazard = !1,
		this.leftLights = new b.Light,
		this.rightLights = new b.Light,
		this.state = b._TurnSignals.State.off,
		this.startTime = (new Date).getTime(),
		this.notchIntervalAngle = 2.0943951023931957
	},
	b.TurnSignals.__name__ = !0,
	b.TurnSignals.prototype = {
		getRightBrightness: function () {
			return this.rightLights.brightness
		},
		getLeftBrightness: function () {
			return this.leftLights.brightness
		},
		update: function (e, t) {
			this.steeringAngle = t;
			if (this.state != b._TurnSignals.State.off || this.enabledHazard) {
				var n = Math.floor(((new Date).getTime() - this.startTime) / this.intervalTime) % 2 == 0;
				this.blinkCircuit != n && (this.blinkCircuit = n, this.blinkCircuit ? r.playSE("TurnSignalsLightOn") : r.playSE("TurnSignalsLightOff"))
			}
			var i = this.notchArea;
			this.notchArea = Math.round(t / this.notchIntervalAngle),
			this.state != b._TurnSignals.State.off && this.notchArea != i && (this.state == b._TurnSignals.State.left && this.notchArea > i || this.state == b._TurnSignals.State.right && this.notchArea < i ? (this.notchCount++, this.notchCount >= 2 ? this.turnOff() : r.playSE("TurnSignalsNotch")) : (this.notchCount = 0, r.playSE("TurnSignalsNotch"))),
			this.leftLights.turnOff(),
			this.rightLights.turnOff(),
			this.blinkCircuit && (this.state != b._TurnSignals.State.off && (this.state == b._TurnSignals.State.left && this.leftLights.turnOn(), this.state == b._TurnSignals.State.right && this.rightLights.turnOn()), this.enabledHazard && (this.leftLights.turnOn(), this.rightLights.turnOn())),
			this.leftLights.update(e),
			this.rightLights.update(e)
		},
		toggleHazard: function () {
			r.playSE("HazardButton"),
			this.enabledHazard == 0 ? (this.enabledHazard = !0, this.state == b._TurnSignals.State.off && (this.startTime = (new Date).getTime(), this.blinkCircuit = !1)) : this.enabledHazard = !1
		},
		turnOff: function () {
			this.state = b._TurnSignals.State.off,
			r.playSE("TurnSignalsCancel")
		},
		turnOn: function (e) {
			this.state = e,
			this.enabledHazard == 0 && (this.startTime = (new Date).getTime(), this.blinkCircuit = !1),
			this.notchArea = Math.round(this.steeringAngle / this.notchIntervalAngle),
			this.notchCount = 0,
			r.playSE("TurnSignalsOn")
		},
		cancel: function () {
			this.state != b._TurnSignals.State.off && this.turnOff()
		},
		switchRight: function () {
			if (this.state != b._TurnSignals.State.off) {
				this.turnOff();
				return
			}
			this.turnOn(b._TurnSignals.State.right)
		},
		switchLeft: function () {
			if (this.state != b._TurnSignals.State.off) {
				this.turnOff();
				return
			}
			this.turnOn(b._TurnSignals.State.left)
		},
		__class__: b.TurnSignals
	},
	b.Vehicle = function (e, t, n, r, i, s, o) {
		this.dTilt = 0,
		this.roll = 0,
		this.tilt = 0,
		this.squealLevel = 0,
		this.gearChangeTime = 0,
		this.brakeLock = !0,
		this.spec = n,
		this.automatic = o,
		this.steering = new b.Steering(440, 12.5),
		this.gearBox = new b.GearBox,
		this.brakeLights = new b.Light,
		this.reverseLights = new b.Light,
		this.turnSignals = new b.TurnSignals,
		this.accelerometer = new b.Accelerometer,
		this.gVector = new v.geoquake.geometry.Vector3(0, 0, 0);
		var u = new d.dynamics.B2BodyDef;
		u.type = d.dynamics.B2Body.b2_dynamicBody,
		u.position.set(r, i),
		u.angle = s;
		var a = new d.collision.shapes.B2PolygonShape;
		a.setAsVector(t),
		this.body = e.createBody(u),
		this.body.createFixture2(a);
		var f = new d.collision.shapes.B2MassData;
		f.I = 3500,
		f.mass = 1500,
		f.center = new d.common.math.B2Vec2(0, 0),
		this.body.setMassData(f);
		var l = n.rearY - n.wheelBase;
		this.wheels = [],
		this.wheels.push(new b.Wheel(this, "Front Left", -n.tread / 2, l, .1, 0, s)),
		this.wheels.push(new b.Wheel(this, "Front Right", n.tread / 2, l, -0.1, 0, s)),
		this.wheels.push(new b.Wheel(this, "Rear Left", -n.tread / 2, n.rearY, .1, 0, s)),
		this.wheels.push(new b.Wheel(this, "Rear Right", n.tread / 2, n.rearY, -0.1, 0, s))
	},
	b.Vehicle.__name__ = !0,
	b.Vehicle.prototype = {
		getWheelPosition: function (e) {
			return this.getWheel(e).getPosition()
		},
		getWheel: function (e) {
			if (e < 0 || e >= this.wheels.length) throw "Out of wheel index";
			return this.wheels[e]
		},
		getWheelNumber: function () {
			return this.wheels.length
		},
		getGVector: function () {
			return this.gVector
		},
		getSpeedMph: function () {
			return b.Box2DUtils.toKmh(Math.abs(this.speed)) / 1.609344
		},
		getSpeedKmh: function () {
			return this.speedKmh
		},
		setAttitude: function (e, t) {
			this.dTilt = (e - this.tilt) * 57.29577951308232,
			this.tilt = e,
			this.roll = t
		},
		getSquealLevel: function () {
			return this.squealLevel
		},
		getSpec: function () {
			return this.spec
		},
		getTurnSignalsRightBrightness: function () {
			return this.turnSignals.getRightBrightness()
		},
		getTurnSignalsLeftBrightness: function () {
			return this.turnSignals.getLeftBrightness()
		},
		getReverseLightsBrightness: function () {
			return this.reverseLights.brightness
		},
		getBrakeLightsBrightness: function () {
			return this.brakeLights.brightness
		},
		getHeading: function () {
			return this.body.getAngle()
		},
		getSteeringAngle: function () {
			return this.steering.angle
		},
		getTurningRadius: function () {
			return this.turningRadius
		},
		getAirDragForce: function () {
			return this.airDragForce
		},
		getMass: function () {
			return this.body.getMass()
		},
		getPosition: function () {
			var e = this.body.getWorldCenter();
			return new v.geoquake.geometry.Vector3(e.x, e.y, 0)
		},
		getGear: function () {
			return this.gearBox.getCurrentGear()
		},
		lateUpdate: function (e) {
			var t = this.body.getWorldCenter(),
			n = new v.geoquake.geometry.Vector3(t.x, t.y, 0);
			this.accelerometer.update(e, n),
			this.gVector = this.accelerometer.getVector(),
			this.gVector.y = -this.gVector.y,
			this.gVector.rotateZ(this.body.getAngle() + 3.141592653589794)
		},
		update: function (e) {
			this.speed = this.body.getLinearVelocity().length(),
			this.speedKmh = b.Box2DUtils.toKmh(this.speed);
			var t = (this.wheels[0].slipAngle + this.wheels[1].slipAngle) / 2 * 57.29577951308232;
			this.steering.update(e, this.speedKmh, t);
			var n = this.steering.getRudderAngle(),
			r = Math.tan(Math.PI / 2 - n) * this.spec.wheelBase;
			this.frontLAngle = Math.atan(this.spec.wheelBase / (r + this.spec.tread / 2)),
			this.frontRAngle = Math.atan(this.spec.wheelBase / (r - this.spec.tread / 2));
			var i = Math.abs(r) + this.spec.tread / 2,
			s = this.spec.wheelBase,
			o = Math.sqrt(i * i + s * s);
			this.wheels[0].setAngle(this.frontLAngle),
			this.wheels[1].setAngle(this.frontRAngle);
			var u = d.common.math.B2Math.dot(this.body.getLinearVelocity(), b.Box2DUtils.getFrontVector(this.body));
			if (this.automatic) {
				var a = this.gearBox.getCurrentGear();
				switch (a) {
				case 1:
					u < .3 && this.brake > .3 ? (this.brakeLock = !0, this.gearChangeTime += e, this.gearChangeTime > .4 && this.gearBox.gearDown()) : this.gearChangeTime = 0,
					this.throttle > 0 && (this.brakeLock = !1),
					this.brakeLock && (this.brake = 1);
					break;
				case - 1 : u > -0.3 && this.throttle > .3 ? (this.brakeLock = !0, this.gearChangeTime += e, this.gearChangeTime > .4 && this.gearBox.gearUp()) : this.gearChangeTime = 0;
					var f = this.throttle;
					this.throttle = this.brake,
					this.brake = f,
					this.throttle > 0 && (this.brakeLock = !1),
					this.brakeLock && (this.brake = 1)
				}
			}
			var l = Math.abs(this.dTilt * 1e3),
			c = this.throttle * 2e3 * this.gearBox.getGearRatio(),
			h = this.brake * 2e3;
			this.wheels[2].drivingForce = c,
			this.wheels[3].drivingForce = c,
			this.wheels[0].brakeForce = h + 25 + l,
			this.wheels[1].brakeForce = h + 25 + l,
			this.wheels[2].brakeForce = h + 25 + l,
			this.wheels[3].brakeForce = h + 25 + l;
			var p = -this.gVector.y * .025,
			v = this.gVector.x * .015;
			this.wheels[0].set_loadFactor(1 - p - v),
			this.wheels[1].set_loadFactor(1 - p + v),
			this.wheels[2].set_loadFactor(1 + p - v),
			this.wheels[3].set_loadFactor(1 + p + v),
			this.squealLevel = 0;
			var m = 0,
			a = 0,
			g = this.wheels;
			while (a < g.length) {
				var y = g[a]; ++a,
				y.update(e),
				this.squealLevel += y.getSquealLevel()
			}
			this.squealLevel /= 4,
			this.airDragForce = this.speedKmh * this.speedKmh * (700 / 14400);
			var w = this.body.getLinearVelocity().copy();
			w.negativeSelf(),
			w.normalize(),
			w.multiply(this.airDragForce),
			this.body.applyForce(w, this.body.getWorldCenter());
			var E = this.body.getWorldVector(new d.common.math.B2Vec2(0, -1)),
			S = -this.getMass() * 9.81 * Math.sin(this.tilt);
			E.multiply(S),
			this.body.applyForce(E, this.body.getWorldCenter());
			var x = this.body.getWorldVector(new d.common.math.B2Vec2(1, 0)),
			T = -this.getMass() * 9.81 * Math.sin(this.roll);
			Math.abs(T) > 3e3 && (x.multiply(T), this.body.applyForce(x, this.body.getWorldCenter())),
			this.brake > .3 ? this.brakeLights.turnOn() : this.brakeLights.turnOff(),
			this.brakeLights.update(e),
			this.gearBox.getCurrentGear() == -1 ? this.reverseLights.turnOn() : this.reverseLights.turnOff(),
			this.reverseLights.update(e),
			this.turnSignals.update(e, this.steering.angle)
		},
		toggleHazard: function () {
			this.turnSignals.toggleHazard()
		},
		cancelTurnSignal: function () {
			this.turnSignals.cancel()
		},
		switchRightTurnSignal: function () {
			this.turnSignals.switchRight()
		},
		switchLeftTurnSignal: function () {
			this.turnSignals.switchLeft()
		},
		gearDown: function () {
			this.automatic || this.gearBox.gearDown()
		},
		gearUp: function () {
			this.automatic || this.gearBox.gearUp()
		},
		setBrake: function (e) {
			this.brake = v.geoquake.MyMath.clamp(e, 0, 1)
		},
		setThrottle: function (e) {
			this.throttle = v.geoquake.MyMath.clamp(e, 0, 1)
		},
		setSteeringLever: function (e) {
			this.steering.setLever(e)
		},
		setSteeringDeltaAngle: function (e) {
			this.steering.setDeltaAngle(e)
		},
		__class__: b.Vehicle
	},
	b.VehicleSpec = function () {},
	b.VehicleSpec.__name__ = !0,
	b.VehicleSpec.prototype = {
		__class__: b.VehicleSpec
	},
	b.Wheel = function (e, t, n, r, i, s, o, u) {
		u == null && (u = !0),
		this.loadFactor = 1,
		this.brakeForce = 0,
		this.drivingForce = 0,
		this.name = t,
		this.isDrivingWheel = u,
		this.magicFormula = new b.MagicFormula(.1, 1.4, 4e3, 0);
		var a = new d.dynamics.B2BodyDef;
		a.type = d.dynamics.B2Body.b2_dynamicBody;
		var f = e.body.getPosition(),
		l = new v.geoquake.geometry.Vector2(n, r);
		l.rotate(o),
		a.position.set(f.x + l.x, f.y + l.y),
		a.angle = o;
		var c = new d.collision.shapes.B2PolygonShape;
		c.setAsBox(.08, .3),
		this.body = e.body.getWorld().createBody(a);
		var h = new d.dynamics.B2FixtureDef;
		h.shape = c,
		h.density = 1,
		h.filter.categoryBits = 2,
		h.filter.maskBits = 32767,
		this.body.createFixture(h);
		var p = new d.dynamics.joints.B2RevoluteJointDef;
		p.enableLimit = !0,
		p.lowerAngle = 0,
		p.upperAngle = 0,
		p.bodyA = e.body,
		p.bodyB = this.body,
		p.localAnchorA.set(n, r),
		p.localAnchorB.set(i, s),
		this.joint = e.body.getWorld().createJoint(p)
	},
	b.Wheel.__name__ = !0,
	b.Wheel.prototype = {
		update: function (e) {
			var t = this.body.getWorldCenter(),
			n = this.body.getLinearVelocity(),
			r = b.Box2DUtils.getFrontVector(this.body);
			if (this.isDrivingWheel) {
				var i = d.common.math.B2Math.mulFV(this.drivingForce, r);
				this.body.applyForce(i, t)
			}
			var s = d.common.math.B2Math.dot(n, r),
			o = v.geoquake.MyMath.clamp(s * 4 / (e * 60), -1, 1),
			u = d.common.math.B2Math.mulFV( - this.brakeForce * o, r);
			this.body.applyForce(u, t);
			var a = r.length() * n.length();
			this.slipAngle = 0,
			a != 0 && (this.slipAngle = d.common.math.B2Math.crossVV(r, n) / a);
			var f = this.magicFormula.calc(this.slipAngle * 57.29577951308232);
			f *= this.loadFactor;
			if (f != 0) {
				var l = Math.min(n.length() / (e * 60), 1),
				c = this.body.getWorldVector(new d.common.math.B2Vec2( - 1, 0));
				c.multiply(f * l),
				this.body.applyForce(c, t);
				var h = Math.abs(d.common.math.B2Math.dot(n, c));
				this.squealLevel = v.geoquake.MyMath.clamp(v.geoquake.MyMath.linearMap(h, 1e4, 3e4, 0, 1), 0, 1)
			}
		},
		set_loadFactor: function (e) {
			return this.loadFactor = v.geoquake.MyMath.clamp(e, 0, 2),
			this.loadFactor
		},
		getPosition: function () {
			var e = this.body.getPosition();
			return new v.geoquake.geometry.Vector2(e.x, e.y)
		},
		getSquealLevel: function () {
			return this.squealLevel
		},
		setAngle: function (e) {
			this.joint.setLimits(e, e)
		},
		__class__: b.Wheel
	};
	var w;
	Array.prototype.indexOf ? i.remove = function (e, t) {
		var n = e.indexOf(t);
		return n == -1 ? !1 : (e.splice(n, 1), !0)
	}: null,
	Math.__name__ = ["Math"],
	Math.NaN = Number.NaN,
	Math.NEGATIVE_INFINITY = Number.NEGATIVE_INFINITY,
	Math.POSITIVE_INFINITY = Number.POSITIVE_INFINITY,
	Math.isFinite = function (e) {
		return isFinite(e)
	},
	Math.isNaN = function (e) {
		return isNaN(e)
	},
	String.prototype.__class__ = String,
	String.__name__ = !0,
	Array.prototype.__class__ = Array,
	Array.__name__ = !0,
	Date.prototype.__class__ = Date,
	Date.__name__ = ["Date"];
	var S = {
		__name__: ["Int"]
	},
	x = {
		__name__: ["Dynamic"]
	},
	T = Number;
	T.__name__ = ["Float"];
	var N = Boolean;
	N.__ename__ = ["Bool"];
	var C = {
		__name__: ["Class"]
	},
	k = {};
	n.DRIVERS_EYE_X = .35,
	n.DRIVERS_EYE_Y = 0,
	n.DRIVERS_EYE_Z = 1.2,
	n.STEERING_OFFSET_Y = .75,
	n.STEERING_OFFSET_Z = -0.32,
	n.VIEWPOINT_DRIVER = 0,
	n.VIEWPOINT_RC = 3,
	n.VIEWPOINT_TOPDOWN = 4,
	n.GRAVITY_X = 616,
	n.GRAVITY_Y = 210,
	n.GRAVITY_MARKER_X = 664,
	n.GRAVITY_MARKER_Y = 258,
	s.EARTH_RADIUS = 6371e3,
	p.DEAD_ZONE = .3,
	d.collision.B2Collision.b2_nullFeature = 255,
	d.collision.B2Collision.s_incidentEdge = d.collision.B2Collision.makeClipPointVector(),
	d.collision.B2Collision.s_clipPoints1 = d.collision.B2Collision.makeClipPointVector(),
	d.collision.B2Collision.s_clipPoints2 = d.collision.B2Collision.makeClipPointVector(),
	d.collision.B2Collision.s_edgeAO = new Array,
	d.collision.B2Collision.s_edgeBO = new Array,
	d.collision.B2Collision.s_localTangent = new d.common.math.B2Vec2,
	d.collision.B2Collision.s_localNormal = new d.common.math.B2Vec2,
	d.collision.B2Collision.s_planePoint = new d.common.math.B2Vec2,
	d.collision.B2Collision.s_normal = new d.common.math.B2Vec2,
	d.collision.B2Collision.s_tangent = new d.common.math.B2Vec2,
	d.collision.B2Collision.s_tangent2 = new d.common.math.B2Vec2,
	d.collision.B2Collision.s_v11 = new d.common.math.B2Vec2,
	d.collision.B2Collision.s_v12 = new d.common.math.B2Vec2,
	d.collision.B2Collision.b2CollidePolyTempVec = new d.common.math.B2Vec2,
	d.collision.B2Distance.s_simplex = new d.collision.B2Simplex,
	d.collision.B2Distance.s_saveA = new Array,
	d.collision.B2Distance.s_saveB = new Array,
	d.collision.B2DynamicTreeNode.currentID = 0,
	d.collision.B2Manifold.e_circles = 1,
	d.collision.B2Manifold.e_faceA = 2,
	d.collision.B2Manifold.e_faceB = 4,
	d.collision.B2SeparationFunction.e_points = 1,
	d.collision.B2SeparationFunction.e_faceA = 2,
	d.collision.B2SeparationFunction.e_faceB = 4,
	d.collision.B2TimeOfImpact.b2_toiCalls = 0,
	d.collision.B2TimeOfImpact.b2_toiIters = 0,
	d.collision.B2TimeOfImpact.b2_toiMaxIters = 0,
	d.collision.B2TimeOfImpact.b2_toiRootIters = 0,
	d.collision.B2TimeOfImpact.b2_toiMaxRootIters = 0,
	d.collision.B2TimeOfImpact.s_cache = new d.collision.B2SimplexCache,
	d.collision.B2TimeOfImpact.s_distanceInput = new d.collision.B2DistanceInput,
	d.collision.B2TimeOfImpact.s_xfA = new d.common.math.B2Transform,
	d.collision.B2TimeOfImpact.s_xfB = new d.common.math.B2Transform,
	d.collision.B2TimeOfImpact.s_fcn = new d.collision.B2SeparationFunction,
	d.collision.B2TimeOfImpact.s_distanceOutput = new d.collision.B2DistanceOutput,
	d.collision.shapes.B2Shape.e_unknownShape = -1,
	d.collision.shapes.B2Shape.e_circleShape = 0,
	d.collision.shapes.B2Shape.e_polygonShape = 1,
	d.collision.shapes.B2Shape.e_edgeShape = 2,
	d.collision.shapes.B2Shape.e_shapeTypeCount = 3,
	d.collision.shapes.B2Shape.e_hitCollide = 1,
	d.collision.shapes.B2Shape.e_missCollide = 0,
	d.collision.shapes.B2Shape.e_startsInsideCollide = -1,
	d.collision.shapes.B2PolygonShape.s_mat = new d.common.math.B2Mat22,
	d.common.B2Settings.VERSION = "2.1alpha",
	d.common.B2Settings.USHRT_MAX = 65535,
	d.common.B2Settings.b2_pi = Math.PI,
	d.common.B2Settings.b2_maxManifoldPoints = 2,
	d.common.B2Settings.b2_aabbExtension = .1,
	d.common.B2Settings.b2_aabbMultiplier = 2,
	d.common.B2Settings.b2_polygonRadius = 2 * d.common.B2Settings.b2_linearSlop,
	d.common.B2Settings.b2_linearSlop = .005,
	d.common.B2Settings.b2_angularSlop = 2 / 180 * d.common.B2Settings.b2_pi,
	d.common.B2Settings.b2_toiSlop = 8 * d.common.B2Settings.b2_linearSlop,
	d.common.B2Settings.b2_maxTOIContactsPerIsland = 32,
	d.common.B2Settings.b2_maxTOIJointsPerIsland = 32,
	d.common.B2Settings.b2_velocityThreshold = 1,
	d.common.B2Settings.b2_maxLinearCorrection = .2,
	d.common.B2Settings.b2_maxAngularCorrection = 8 / 180 * d.common.B2Settings.b2_pi,
	d.common.B2Settings.b2_maxTranslation = 6,
	d.common.B2Settings.b2_maxTranslationSquared = d.common.B2Settings.b2_maxTranslation * d.common.B2Settings.b2_maxTranslation,
	d.common.B2Settings.b2_maxRotation = .5 * d.common.B2Settings.b2_pi,
	d.common.B2Settings.b2_maxRotationSquared = d.common.B2Settings.b2_maxRotation * d.common.B2Settings.b2_maxRotation,
	d.common.B2Settings.b2_contactBaumgarte = .2,
	d.common.B2Settings.b2_timeToSleep = .5,
	d.common.B2Settings.b2_linearSleepTolerance = .01,
	d.common.B2Settings.b2_angularSleepTolerance = 2 / 180 * d.common.B2Settings.b2_pi,
	d.common.math.B2Math.b2Vec2_zero = new d.common.math.B2Vec2(0, 0),
	d.common.math.B2Math.b2Mat22_identity = d.common.math.B2Mat22.fromVV(new d.common.math.B2Vec2(1, 0), new d.common.math.B2Vec2(0, 1)),
	d.common.math.B2Math.b2Transform_identity = new d.common.math.B2Transform(d.common.math.B2Math.b2Vec2_zero, d.common.math.B2Math.b2Mat22_identity),
	d.common.math.B2Math.MIN_VALUE = 2.2250738585072014e-308,
	d.common.math.B2Math.MAX_VALUE = 1.7976931348623157e308,
	d.dynamics.B2Body.s_xf1 = new d.common.math.B2Transform,
	d.dynamics.B2Body.e_islandFlag = 1,
	d.dynamics.B2Body.e_awakeFlag = 2,
	d.dynamics.B2Body.e_allowSleepFlag = 4,
	d.dynamics.B2Body.e_bulletFlag = 8,
	d.dynamics.B2Body.e_fixedRotationFlag = 16,
	d.dynamics.B2Body.e_activeFlag = 32,
	d.dynamics.B2Body.b2_staticBody = 0,
	d.dynamics.B2Body.b2_kinematicBody = 1,
	d.dynamics.B2Body.b2_dynamicBody = 2,
	d.dynamics.B2ContactFilter.b2_defaultFilter = new d.dynamics.B2ContactFilter,
	d.dynamics.B2ContactListener.b2_defaultListener = new d.dynamics.B2ContactListener,
	d.dynamics.B2ContactManager.s_evalCP = new d.collision.B2ContactPoint,
	d.dynamics.B2DebugDraw.e_shapeBit = 1,
	d.dynamics.B2DebugDraw.e_jointBit = 2,
	d.dynamics.B2DebugDraw.e_aabbBit = 4,
	d.dynamics.B2DebugDraw.e_pairBit = 8,
	d.dynamics.B2DebugDraw.e_centerOfMassBit = 16,
	d.dynamics.B2DebugDraw.e_controllerBit = 32,
	d.dynamics.B2Island.s_impulse = new d.dynamics.B2ContactImpulse,
	d.dynamics.B2World.s_timestep2 = new d.dynamics.B2TimeStep,
	d.dynamics.B2World.s_xf = new d.common.math.B2Transform,
	d.dynamics.B2World.s_backupA = new d.common.math.B2Sweep,
	d.dynamics.B2World.s_backupB = new d.common.math.B2Sweep,
	d.dynamics.B2World.s_timestep = new d.dynamics.B2TimeStep,
	d.dynamics.B2World.s_queue = new Array,
	d.dynamics.B2World.s_jointColor = new d.common.B2Color(.5, .8, .8),
	d.dynamics.B2World.e_newFixture = 1,
	d.dynamics.B2World.e_locked = 2,
	d.dynamics.contacts.B2Contact.e_sensorFlag = 1,
	d.dynamics.contacts.B2Contact.e_continuousFlag = 2,
	d.dynamics.contacts.B2Contact.e_islandFlag = 4,
	d.dynamics.contacts.B2Contact.e_toiFlag = 8,
	d.dynamics.contacts.B2Contact.e_touchingFlag = 16,
	d.dynamics.contacts.B2Contact.e_enabledFlag = 32,
	d.dynamics.contacts.B2Contact.e_filterFlag = 64,
	d.dynamics.contacts.B2Contact.s_input = new d.collision.B2TOIInput,
	d.dynamics.contacts.B2PositionSolverManifold.circlePointA = new d.common.math.B2Vec2,
	d.dynamics.contacts.B2PositionSolverManifold.circlePointB = new d.common.math.B2Vec2,
	d.dynamics.contacts.B2ContactSolver.s_worldManifold = new d.collision.B2WorldManifold,
	d.dynamics.contacts.B2ContactSolver.s_psm = new d.dynamics.contacts.B2PositionSolverManifold,
	d.dynamics.joints.B2Joint.e_unknownJoint = 0,
	d.dynamics.joints.B2Joint.e_revoluteJoint = 1,
	d.dynamics.joints.B2Joint.e_prismaticJoint = 2,
	d.dynamics.joints.B2Joint.e_distanceJoint = 3,
	d.dynamics.joints.B2Joint.e_pulleyJoint = 4,
	d.dynamics.joints.B2Joint.e_mouseJoint = 5,
	d.dynamics.joints.B2Joint.e_gearJoint = 6,
	d.dynamics.joints.B2Joint.e_lineJoint = 7,
	d.dynamics.joints.B2Joint.e_weldJoint = 8,
	d.dynamics.joints.B2Joint.e_frictionJoint = 9,
	d.dynamics.joints.B2Joint.e_inactiveLimit = 0,
	d.dynamics.joints.B2Joint.e_atLowerLimit = 1,
	d.dynamics.joints.B2Joint.e_atUpperLimit = 2,
	d.dynamics.joints.B2Joint.e_equalLimits = 3,
	d.dynamics.joints.B2PulleyJoint.b2_minPulleyLength = 2,
	d.dynamics.joints.B2RevoluteJoint.tImpulse = new d.common.math.B2Vec2,
	v.geoquake.Angle.PI2 = 6.283185307179586,
	v.geoquake.js.Debug.WATCH = "debug_watch",
	v.geoquake.js.Debug.LOG = "debug_log",
	v.geoquake.js.Debug.logLine = 0,
	v.geoquake.js.Debug.logHeight = 20,
	v.geoquake.js.Debug.watchParams = new g.ds.StringMap,
	y.Browser.window = typeof window != "undefined" ? window: null,
	y.Browser.document = typeof window != "undefined" ? window.document: null,
	b.GearBox.DRIVE = 1,
	b.GearBox.REVERSE = -1,
	b.TurnSignals.AUTOCANCEL_COUNT = 2,
	b.Vehicle.GEAR_CHANGE_TARGET = .4,
	b.Vehicle.ROLLING_DRAG_FORCE = 25,
	b.Vehicle.BUMP_DRAG_FACTOR = 1e3,
	u.main()
})();
