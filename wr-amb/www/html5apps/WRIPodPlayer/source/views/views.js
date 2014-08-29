/**
	For simple applications, you might define all of your views in this file.  
	For more complex applications, you might choose to separate these kind definitions 
	into multiple files under this folder.
*/

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
										//{kind: "enyo.Image", src: "http://enyojs.com/img/enyo-logo.png", alt: "enyo logo"},
										{kind: "enyo.Scroller", fit: true, components: [
											{name: "main", classes: "nice-padding", allowHtml: true}
										]},
										{kind: "onyx.ProgressBar", progress: 25, showStripes: false},
										{kind: "FittableRows", classes: "enyo-center", components: [
											{content: "~song title~", style: "text-align: center"},
											{content: "~artist and album name~", style: "text-align: center"},
										]},
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

	},

	prevOnTap: function(inSender, inEvent) {
		this.$.main.addContent("prevOntap.<br/>");
		this.$.progressBar.animateProgressTo(25);
	},

	playOnTap: function(inSender, inEvent) {
		var text = this.$.playButton.getContent();
		this.$.main.addContent(text + "Ontap.<br/>");
		if (text.localeCompare("play") == 0) {
			this.$.playButton.setContent("pause");
		} else {
			this.$.playButton.setContent("play");
		}
	},

	nextOnTap: function(inSender, inEvent) {
		this.$.main.addContent("nextOntap.<br/>");
		this.$.progressBar.animateProgressTo(50);
	},

	listOnTab: function(inSender, inEvent) {
		this.$.main.addContent("listOntap.<br/>");

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
