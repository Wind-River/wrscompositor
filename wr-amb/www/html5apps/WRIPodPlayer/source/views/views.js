var websocket = null;
var playstate = 0;
var updateList = null;
var ipodData = [];

var WRSIPodCategory = Object.freeze({
    TOP : 0,
    PLAYLIST : 1,
    ARTIST : 2,
    ALBUM : 3,
    GENRE : 4,
    TRACK : 5,
    COMPOSER : 6,
    AUDIOBOOK : 7,
    PODCAST : 8,
    NESTED_PLAYLIST : 9,
    GENIUS_MIXES : 10,
    ITUNES_U : 11,
});

// XXX for test
var artworkImage = null;
var main = null;
var playlist = null;

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
                {kind: "enyo.Panels", name: "contentPanels", classes: "enyo-fit", arrangerKind: "CardArranger", onTransitionFinish: "contentTransitionCompleted", components: [
					{kind: "FittableRows", fit: true, components: [
						{tag: "p"},
						{kind: "FittableColumns", fit: true, components: [
							{kind: "FittableColumns", fit: true, components: [
								{kind: "FittableColumns", noStretch: true, fit: true, classes: "song-info-align-center", components: [
									{kind: "enyo.Image", name: "artwork", classes: "artwork-image", src: "assets/default_artwork.jpg", style: "width: 240px; height: 240px; border: 1px solid #797979"},
									{kind: "FittableColumns", components: [
										{kind: "FittableRows", classes: "song-info-center", components: [
											{content: "Title", style: "text-align: left; margin-bottom: 10px"},
											{content: "Album", style: "text-align: left; margin-bottom: 10px"},
											{content: "Artist", style: "text-align: left; margin-bottom: 10px"},
											{fit: true},
										]},
										{kind: "FittableRows", components: [
											{name: "songTitle", content: "", style: "text-align: left; margin-bottom: 10px; color: #ffffff"},
											{name: "songAlbum", content: "", style: "text-align: left; margin-bottom: 10px; color: #ffffff"},
											{name: "songArtist", content: "", style: "text-align: left; margin-bottom: 10px; color: #ffffff"},
											{fit: true},
										]},
									]},
								]},
							]},
						]},
						{kind: "FittableColumns", name: "playinfoLayout", class: "enyo-fit", style: "margin-bottom: 10px", components: [
							{fit: true},
							{content: "Powered by", style: "color: #ffffff; font-size: 14px; vertical-align: bottom"},
							{kind: "enyo.Image", src: "assets/enyo.png", style: "width: 64px; height: 24px; vertical-align: middle; margin-left: 5px"},
							{content: "and", style: "color: #ffffff; font-size: 14px; margin-left: 5px; vertical-align: bottom"},
							{kind: "enyo.Image", src: "assets/wr.png", style: "width: 120px; height: 12px; vertical-align: middle; margin-left: 5px"},
						]},
						{kind: "FittableColumns", name: "playinfoLayout", class: "enyo-fit", components: [
							{name: "songCurrentTime", style: "color: #ffffff; margin-left: 10px"},
							{kind: "FittableRows", class: "enyo-fit", fit: true, components: [
								{kind: "onyx.ProgressBar", name: "timeTrack", progress: 0, showStripes: false},
							]},
							{name: "songTotalTime", style: "color: #ffffff; margin-right: 10px"},
						]},

						{kind: "onyx.Toolbar", layoutKind: "FittableColumnsLayout", noStretch:true, classes: "control-toolbar", components: [
							{kind: "FittableColumns", noStretch: true, components: [
								{kind: "onyx.IconButton", src: "assets/prev.png", ontap: "prevOnTap"},
								{kind: "onyx.IconButton", src: "assets/pause.png", name: "playButton", ontap: "playOnTap"},
								{kind: "onyx.IconButton", src: "assets/next.png", ontap: "nextOnTap"},
							]},
							{fit: true},
							{kind: "onyx.IconButton", name: "listButton", src: "assets/list.png", classes: "list-button", ontap: "listOnTab"},
						]},
					]}
				]}
			]
        },
    ],

    create: function() {
        this.inherited(arguments);
        // XXX
        main = this;

        var wsUrl = "ws://127.0.0.1:9090/";
		if(window.location.hostname != '')
        	wsUrl = "ws://"+window.location.hostname+":9090/";

        websocket = new WebSocket(wsUrl, 'ipod');
		this.reflow();

        websocket.onopen = enyo.bind(this, function(evt) {
		});
        websocket.onclose = function(evt) { console.log('onclose'); };
        websocket.onmessage = enyo.bind(this, handleMessage);
        websocket.onerror = function(evt) { console.log('onopen'); };
    },

    prevOnTap: function(inSender, inEvent) {
        command("prev");
    },

    playOnTap: function(inSender, inEvent) {
        if (playstate == 4) { // playing
            this.$.playButton.setSrc("assets/pause.png");
            command("pause");
        } else if (playstate == 0 || playstate == 5) { // stopped or paused
            this.$.playButton.setSrc("assets/play.png");
            command("play");
        }
    },

    updatePlayPauseButton: function() {
        if (playstate == 4) { // playing
            this.$.playButton.setSrc("assets/pause.png");
        } else if (playstate == 5) { // paused
            this.$.playButton.setSrc("assets/play.png");
        }
    },

    updateArtwork: function(src) {
        this.$.artwork.setSrc(src);
    },

    updateAlbumList: function(data) {
        ipodData = data;
        console.log("updateAlubmList : updateList = " + updateList);
        if (updateList != null) {
            updateList();
            updateList = null;
        }
    },

    nextOnTap: function(inSender, inEvent) {
        command("next");
    },

    listOnTab: function(inSender, inEvent) {
        var albumNum = 3;
        command("get_list", albumNum);
        var newComponent = this.$.contentPanels.createComponent({name: "songList", kind: "songListView"}, {owner: this});
        newComponent.render();
        this.$.contentPanels.render();
        this.$.contentPanels.resize();
        this.$.contentPanels.setIndex(1);
    },

    updateTrackTimeInfo: function(current, total) {
        var currentTime = Math.floor(current / 1000);
        currentTime = sprintf("%02d:%02d", Math.floor(currentTime / 60), (currentTime % 60));
        this.$.songCurrentTime.setContent(currentTime);
        if (total != null) {
            var totalTime = Math.floor(total / 1000);
            totalTime = sprintf("%02d:%02d", Math.floor(totalTime / 60), (totalTime % 60));
            this.$.songTotalTime.setContent(totalTime);
        }
    },

    hideSongList: function(inSender, inEvent) {
        this.hidingSongList = true;
        this.$.contentPanels.setIndex(0);
        //this.updateArtwork('data:image/png;base64,' + artworkImage);
        //console.log("artwork : " + artworkImage);
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
});

enyo.kind({
    name: "songListView",
    kind: "FittableRows",
    classes: "enyo-fit enyo-unselectable",
    data: [],
    events: {
        onHideSongList: ""
    },
    components: [
        {kind: "List",
            name: "list",
            touch: true,
            fit: true,
            classes: "song-listview", onSetupItem: "setupItem", components: [
                {name: "item", ontap: "itemTap", classes: "song-list-item enyo-border-box", components: [
                    {name: "index", style: "float: right;"},
                    {name: "name"},
                ]},
            ],
        },
        {kind: "onyx.Toolbar", layoutKind: "FittableColumnsLayout", noStretch: true, classes: "footer-toolbar", components: [
            {fit: true},
            {kind: "onyx.IconButton", name: "CloseButton", src: "assets/close.png", ontap: "doHideSongList"},
        ]},
    ],

    rendered: enyo.inherit(function(sup) {
        return function() {
            sup.apply(this, arguments);
        };
    }),

    create: function() {
        this.inherited(arguments);
        updateList = enyo.bind(this, refreshList);
        // XXX
        playlist = this;
    },

    getIpodList: function() {
        var listCount = ipodData.length;
        this.data = [];

        for (var i = 0; i < listCount; i++) {
            this.data.push({index: i, name: ipodData[i]});
        }
        console.log("getIpodList: data = " + this.data);
    },

    setupItem: function(inSender, inEvent) {
        var i = inEvent.index;
        var dataItem = this.data[i];
        if (!dataItem) {
            return;
        }

         this.$.item.addRemoveClass("song-list-item-selected", inSender.isSelected(inEvent.index));

        console.log("seupItem: dataItem = " + dataItem.index);
        //this.$.index.setContent(dataItem.index);
        this.$.name.setContent(dataItem.name);
    },

    itemTap: function(inSender, inEvent) {
        console.log("tab item : " + this.data[inEvent.index].name);
        command("select_db", WRSIPodCategory.ALBUM, inEvent.index, 0); // category, album index, track index
        this.doHideSongList();
    }
});

function handleMessage(event) {
    console.log('handle message');
    //console.log('event :' + event.data);
    var obj = JSON.parse(event.data);
    console.log(obj);
    var evt = obj.event;
    var data = obj.data;
    var conn = obj.connection;

    if (conn != null && conn) {
        command('get_trackinfo');
    } else if (evt == "track info") {
        playstate = data.playstate;
        this.updatePlayPauseButton();
        if (playstate == 0) {
            this.updateArtwork("assets/default_artwork.jpg");
        }

        var trackLength = data.track_length;
        this.updateTrackTimeInfo(data.track_position, trackLength);

        this.$.timeTrack.max = trackLength;
        this.$.playinfoLayout.reflow();
        this.$.songTitle.setContent(": " + data.title);
        this.$.songAlbum.setContent(": " + data.album);
        this.$.songArtist.setContent(": " + data.artist);
        command('get_artwork', obj.data.track_index);
    } else if (evt == "current artwork") {
        artworkImage = data.image;
        this.updateArtwork('data:image/png;base64,' + artworkImage);
    } else if (evt == "track position changed") {
        this.$.timeTrack.animateProgressTo(data);
        this.updateTrackTimeInfo(data);
    } else if (evt == "track changed") {
        this.$.timeTrack.animateProgressTo(0);
    } else if (evt == "playstate changed") {
        playstate = data;
        this.updatePlayPauseButton();
    } else if (evt == "album loaded") {
        this.updateAlbumList(data);
    }
}

function command(func) {
    var args = new Array;
    for(var i=1; i<arguments.length; i++)
        args.push(arguments[i]);
    var obj = {"function": func, "args": args};
    console.log(obj);
    websocket.send(JSON.stringify(obj));
}


function refreshList() {
    this.getIpodList();
    this.$.list.setCount(this.data.length);
    this.$.list.reset();
}
