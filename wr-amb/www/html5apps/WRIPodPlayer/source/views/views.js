var websocket = null;
var playstate = 0;
var haha = null;


enyo.kind({
	name: "myapp.MainView",
	classes: "app onyx font-lato enyo-unselectable",
	handlers: {
		onresize: "resized",
		onHideSongList: "hideSongList"
	},
	components:[
		{kind: "enyo.Panels",
			name: "mainPanels",
			classes: "panels enyo-fit",

			components: [
				{kind: "enyo.Panels", name: "headerPanels", classes: "enyo-fit",
					components: [
						{kind: "FittableRows", name: "headerRows", classes: "enyo-fit", components: [
							{kind: "onyx.Toolbar", name: "headerToolbar", layoutKind: "FittableColumnsLayout", components: [
								{content: "iPOD Player"},
								{fit: true},
								{kind: "onyx.Button", name: "listButton", content: "List", ontap: "listOnTab"},
							]},
							{kind: "enyo.Panels", name: "contentPanels", classes: "content-panel", fit: true,
								arrangerKind: "CardArranger", onTransitionFinish: "contentTransitionCompleted", 
								components: [
									{kind: "FittableRows", name: "controller", classes: "wide", fit: true, components: [
										{kind: "FittableColumns", noStretch: true, fit: true, classes: "song-info-align-center", components: [
											{kind: "enyo.Image", name: "artwork", classes: "artwork-image" },
											{kind: "FittableRows", classes: "song-info-center", components: [
												{name: "songTitle", content: "", style: "text-align: left; margin-bottom: 10px"},
												{name: "songAlbum", content: "", style: "text-align: left; margin-bottom: 10px"},
												{name: "songArtist", content: "", style: "text-align: left"},
											]},
										]},
										{kind: "onyx.ProgressBar", name: "timeTrack", progress: 0, showStripes: false},
										{kind: "onyx.Toolbar", layoutKind: "FittableColumnsLayout", classes: "enyo-center", components: [
											{kind: "onyx.Button", content: "<<", ontap: "prevOnTap"},
											{kind: "onyx.Button", name: "playButton", content: "play", ontap: "playOnTap"},
											{kind: "onyx.Button", content: ">>", ontap: "nextOnTap"},
										]},
									]},
								]
							},
						]}
					]
				},
			]
		},
	],

	create: 	function() {
		this.inherited(arguments);
		haha = this;

		var wsUrl = "ws://"+window.location.hostname+":9090/";
		websocket = new WebSocket(wsUrl, 'ipod');

		websocket.onopen = function(evt) { console.log('onopen'); };
		websocket.onclose = function(evt) { console.log('onclose'); };
		websocket.onmessage =  enyo.bind(this, handleMessage);
		websocket.onerror = function(evt) { console.log('onopen'); };
	},

	prevOnTap: function(inSender, inEvent) {
		this.command("prev");
	},

	playOnTap: function(inSender, inEvent) {
		if (playstate == 4) { // playing
			this.$.playButton.setContent("pause");
			this.command("pause");
		} else if (playstate == 0 || playstate == 5) { // stopped or paused
			this.$.playButton.setContent("play");
			this.command("play");
		}
	},

	updatePlayPauseButton: function() {
		if (playstate == 4) { // playing
			this.$.playButton.setContent("pause");
		} else if (playstate == 5) { // paused
			this.$.playButton.setContent("play");
		}
	},

	updateArtwork: function(src) {
		this.$.artwork.setSrc(src);
	},

	nextOnTap: function(inSender, inEvent) {
		this.command("next");
	},

	listOnTab: function(inSender, inEvent) {
		var text = this.$.listButton.getContent();
		if (text.localeCompare("List") == 0) {
			this.$.listButton.setContent("Close");
			this.$.headerRows.resize();

			var newComponent = this.$.contentPanels.createComponent({name: "songList", kind: "songListView"}, {owner: this});
			newComponent.render();
			this.$.contentPanels.render();
			this.$.contentPanels.setIndex(1);
		} else {
			this.$.listButton.setContent("List");
			this.hideSongList();
		}

	},

	hideSongList: function(inSender, inEvent) {
		this.hidingSongList = true;
		this.$.contentPanels.setIndex(0);
	},

	resized: function() {
		console.log("resized");
	},

	contentTransitionCompleted: function(inSender, inEvent) {
		if (this.hidingSongList) {
			this.destroySongList();
		}
	},

	destroySongList: function() {
		this.$.songList.destroy();
		this.hidingSongList = false;
	},


	command: function(func) {
		var args = new Array;
		for(var i=1; i<arguments.length; i++)
			args.push(arguments[i]);
		var obj = {"function": func, "args": args};
		console.log(obj);
		websocket.send(JSON.stringify(obj));
	},

});

enyo.kind({
	name: "songListView",
	kind: "FittableRows",
	events: {
		onHideSongList: ""
	},
	components: [
		{content: "list..."},
		/*
		{kind: "onyx.Toolbar", layoutKind: "FittableColumnsLayout", fit: true, components: [
			{kind: "onyx.Button", content: "Close", ontap: "doHideSongList"}
		]}
		*/
	],
});

function handleMessage(event) {
	console.log('handle message');
	console.log('event :' + event.data);
	var obj = JSON.parse(event.data);
	console.log(obj);
	var evt = obj.event;
	var data = obj.data;
	var conn = obj.connection;

	if (conn != null && conn) {
		this.command('get_trackinfo');
	} else if (evt == "track info") {
		playstate = data.playstate;
		this.updatePlayPauseButton();
		if (playstate == 0) {
			this.updateArtwork("assets/default_artwork.jpg");
		}

		this.$.timeTrack.max = data.track_length;
		this.$.songTitle.setContent("title : " + data.title);
		this.$.songAlbum.setContent("album : " + data.album);
		this.$.songArtist.setContent("artist : " + data.artist);
		this.command('get_artwork', obj.data.track_index);
	} else if (evt == "current artwork") {
		this.updateArtwork('data:image/png;base64,' + data.image);
	} else if (evt == "track position changed") {
		this.$.timeTrack.animateProgressTo(data);
	} else if (evt == "track changed") {
		this.$.timeTrack.animateProgressTo(0);
	} else if (evt == "playstate changed") {
		playstate = data;
		this.updatePlayPauseButton();
	}
}
