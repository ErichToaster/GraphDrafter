#include <iostream>
#include <vector>
#include <cairo/cairo.h> //potentially change this to <cairo.h>
#include <cmath>

//to compile, add "-lcairo" flag

bool boolPrompt(std::string query) {
	std::string userInput = "";
	std::cout << "[Graph Drafter]: ";
	std::cout << query << " <y/n>: ";
	getline(std::cin, userInput);
	if(
		userInput == "y"	||
		userInput == "Y"	||
		userInput == "yes"	||
		userInput == "Yes"	||
		userInput == "YES"
	) {return true;}
	return false;
}

struct edge {
	int nodeA;
	int nodeB;
};

std::vector<edge> getEdges(std::vector<std::vector<bool>> edges) {
	std::vector<edge> toReturn = {};
	for(int i = 0; i < edges.size(); i++) {
		for(int j = 0; j < edges[i].size(); j++) {
			if(!edges[i][j] || i <= j) {continue;}
			toReturn.push_back({i, j});
		}
	}
	return toReturn;
}

void printNames(std::vector<std::string> names) {
	int amountOfSpaces = 0;
	for(int i = 0; i < names.size(); i++) {
		
		std::cout << "[";
		amountOfSpaces =
		std::to_string(names.size()-1).length() -
		std::to_string(i).length();
		for(int j = 0; j < amountOfSpaces; j++) {std::cout << " ";}
		std::cout << i << "]: " << names[i] << "\n";
	}
}

void printEdges(std::vector<std::string> names, std::vector<edge> edges) {
	int amountOfSpaces = 0;
	int longestName = 0;
	int nodeA = 0;
	int nodeB = 0;

	for(int i = 0; i < names.size(); i++) {
		if(names[i].length() > longestName) {longestName = names[i].length();}
	}
	
	for(int i = 0; i < edges.size(); i++) {
		nodeA = edges[i].nodeA;
		nodeB = edges[i].nodeB;
		
		//index of current edge
		std::cout << "[[EDGE ";
		amountOfSpaces =
		std::to_string(edges.size()-1).length() -
		std::to_string(i).length();
		for(int j = 0; j < amountOfSpaces; j++) {std::cout << " ";}
		std::cout << i << "]]: ";
		
		//index and name of node A
		std::cout << "[";

		amountOfSpaces =
		std::to_string(names.size()-1).length() -
		std::to_string(nodeA).length();
		for(int j = 0; j < amountOfSpaces; j++) {std::cout << " ";}

		std::cout << nodeA << "]: " << names[nodeA];
		
		//index and name of node B
		amountOfSpaces =
		longestName -
		names[nodeA].length();
		for(int j = 0; j < amountOfSpaces; j++) {std::cout << " ";}

		std::cout << " <---> [";

		amountOfSpaces =
		std::to_string(names.size()-1).length() -
		std::to_string(nodeB).length();
		for(int j = 0; j < amountOfSpaces; j++) {std::cout << " ";}

		std::cout << nodeB << "]: " << names[nodeB] << "\n";
	}
}

int nodeSelectPrompt(std::string promptText, std::vector<std::string> nodes) {
	std::string userInput = "";
	printNames(nodes);
	std::cout << promptText << "<0-" << nodes.size()-1 << ">: ";
	getline(std::cin, userInput);
	try {
		int toReturn = std::stoi(userInput);
		if(toReturn >= nodes.size()) {return -1;}
		return toReturn;
	} catch(const std::exception& e) {
		return -1;
	}
}

int edgeSelectPrompt(std::string promptText, std::vector<std::string> nodes, std::vector<edge> edges) {
	std::string userInput = "";
	printEdges(nodes, edges);
	std::cout << promptText << "<0-" << edges.size()-1 << ">: ";
	getline(std::cin, userInput);
	try {
		int toReturn = std::stoi(userInput);
		if(toReturn >= edges.size()) {return -1;}
		return toReturn;
	} catch(const std::exception& e) {
		return -1;
	}
}

void print_help() {
	std::cout
	<< "X===============================X \n"
	<< "|        [GRAPH DRAFTER]        | \n"
	<< "|-------------------------------| \n"
	<< "|-help   - display this sheet   | \n"
	<< "|-save   - save and exit        | \n"
	<< "|-exit   - save and exit        | \n"
	<< "|-cancel - exit without saving  | \n"
	<< "|-------------------------------| \n"
	<< "|-mknode - add a node           | \n"
	<< "|-rmnode - remove a node        | \n"
	<< "|-lsnode - show list of nodes   | \n"
	<< "|-------------------------------| \n"
	<< "|-mkedge - add an edge          | \n"
	<< "|-rmedge - remove an edge       | \n"
	<< "|-lsedge - show list of edges   | \n"
	<< "X===============================X \n"
	<< std::endl;
	return;
}

enum instruction_type {
	INST_SAVE,
	INST_CANCEL,
	INST_MAKE_NODE,
	INST_REMOVE_NODE,
	INST_LIST_NODES,
	INST_MAKE_EDGE,
	INST_REMOVE_EDGE,
	INST_LIST_EDGES,
	INST_INVALID
};

instruction_type parseInstruction(std::string input) {
	//the help instruction isn't actually parsed
	//anything invalid prints the help sheet
	if(
		input == "exit" ||
		input == "quit" ||
		input == "save"
	) {				return INST_SAVE;}
	else if(input == "cancel") {	return INST_CANCEL;}
	else if(input == "mknode") {	return INST_MAKE_NODE;}
	else if(input == "rmnode") {	return INST_REMOVE_NODE;}
	else if(input == "lsnode") {	return INST_LIST_NODES;}
	else if(input == "mkedge") {	return INST_MAKE_EDGE;}
	else if(input == "rmedge") {	return INST_REMOVE_EDGE;}
	else if(input == "lsedge") {	return INST_LIST_EDGES;}
	else {				return INST_INVALID;}
}

void mknode(std::vector<std::string> *names, std::vector<std::vector<bool>> *edges) {
	bool error = false;

	//get user input
	std::string userInput = "";
	std::cout << "[Graph Drafter /mknode]: Enter node name: ";
	getline(std::cin, userInput);
	if(userInput == "") {
		std::cout
		<< "[Graph Drafter /mknode]: "
		<< "Invalid name"
		<< std::endl;
		error = true;
	}
	if(!error) {
		for(int i = 0; i < (*names).size(); i++) {
			if((*names)[i] != userInput) {continue;}
			std::cout
			<< "[Graph Drafter /mknode]: "
			<< "That node already exists"
			<< std::endl;
			error = true;
			break;
		}
	}

	//apply user input
	if(!error) {
		(*names).push_back(userInput);
		for(int i = 0; i < (*edges).size(); i++) {(*edges)[i].push_back(false);}
		(*edges).push_back(std::vector<bool>((*edges).size()+1, false));
	}

	//finalize
	std::cout << "[Graph Drafter]: ";
	if(!error) {
		std::cout << "Successfully created node!";
	} else {
		std::cout << "Failed to create node.";
	}
	std::cout << std::endl;
	return;
}

void rmnode(std::vector<std::string> *names, std::vector<std::vector<bool>> *edges) {
	bool error = false;

	//get user input
	int selectedNode = -1;
	if((*names).empty()) {
		std::cout
		<< "[Graph Drafter /rmnode]: "
		<< "No nodes to remove"
		<< std::endl;
		error = true;
	}
	if(!error) {
		selectedNode = nodeSelectPrompt("[Graph Drafter /rmnode]: select node ", *names);
		if(selectedNode == -1) {
			std::cout
			<< "[Graph Drafter /rmnode]: "
			<< "Invalid input"
			<< std::endl;
			error = true;
		}
	}

	//apply user input
	if(!error) {
		(*names).erase((*names).begin() + selectedNode);
		(*edges).erase((*edges).begin() + selectedNode);
		for(int i = 0; i < (*edges).size(); i++) {
			(*edges)[i].erase((*edges)[i].begin() + selectedNode);
		}
	}

	//finalize
	std::cout << "[Graph Drafter]: ";
	if(!error) {
		std::cout << "Successfully removed node!";
	} else {
		std::cout << "Failed to remove node.";
	}
	std::cout << std::endl;
	return;
}

void mkedge(std::vector<std::string> names, std::vector<std::vector<bool>> *edges) {
	bool error = false;

	//get user input
	int selectedNodeA = -1;
	int selectedNodeB = -1;
	if((names).size() < 2) {
		std::cout
		<< "[Graph Drafter /mkedge]: "
		<< "Not enough nodes to create an edge"
		<< std::endl;
		error = true;
	}
	if(!error) {
		selectedNodeA = nodeSelectPrompt("[Graph Drafter /mkedge]: select first node ", names);
		if(selectedNodeA == -1) {
			std::cout
			<< "[Graph Drafter /mkedge]: "
			<< "Invalid input"
			<< std::endl;
			error = true;
		}
	}
	if(!error) {
		selectedNodeB = nodeSelectPrompt("[Graph Drafter /mkedge]: select second node ", names);
		if(selectedNodeB == -1) {
			std::cout
			<< "[Graph Drafter /mkedge]: "
			<< "Invalid input"
			<< std::endl;
			error = true;
		} else if(selectedNodeA == selectedNodeB) {
			std::cout
			<< "[Graph Drafter /mkedge]: "
			<< "Cannot connect a node to itself"
			<< std::endl;
			error = true;
		}
	}

	//apply user input
	if(!error) {
		(*edges)[selectedNodeA][selectedNodeB] = true;
		(*edges)[selectedNodeB][selectedNodeA] = true;
	}
	
	//finalize
	std::cout << "[Graph Drafter]: ";
	if(!error) {
		std::cout << "Successfully created edge!";
	} else {
		std::cout << "Failed to create edge.";
	}
	std::cout << std::endl;
	return;
}

void rmedge(std::vector<std::string> names, std::vector<std::vector<bool>> *edges) {
	bool error = false;

	//get user input
	int selectedEdge = -1;
	if((*edges).empty()) {
		std::cout
		<< "[Graph Drafter /rmedge]: "
		<< "No edges to remove"
		<< std::endl;
		error = true;
	}
	if(!error) {
		selectedEdge = edgeSelectPrompt("[Graph Drafter /rmedge]: select edge ", names, getEdges(*edges));
		if(selectedEdge == -1) {
			std::cout
			<< "[Graph Drafter /rmedge]: "
			<< "Invalid input"
			<< std::endl;
			error = true;
		}
	}

	//apply user input
	if(!error) {
		int nodeA = (getEdges(*edges))[selectedEdge].nodeA;
		int nodeB = (getEdges(*edges))[selectedEdge].nodeB;
		(*edges)[nodeA][nodeB] = false;
		(*edges)[nodeB][nodeA] = false;
	}

	//finalize
	std::cout << "[Graph Drafter]: ";
	if(!error) {
		std::cout << "Successfully removed edge!";
	} else {
		std::cout << "Failed to remove edge.";
	}
	std::cout << std::endl;
	return;
	
	return;
}

struct pos {
	int xpos;
	int ypos;
};

int main(int argc, char *argv[]) {
	if(argc != 2) {std::cout << "Usage: " << argv[0] << " <target file>" << std::endl; return 0;}
	std::vector<std::string> names;
	std::vector<std::vector<bool>> edges;
	
	//input loop
	std::string userInput;
	bool brk = false;
	bool cancel = false;
	while(!brk) {
		std::cout << "[Graph Drafter]: ";
		getline(std::cin, userInput);
		switch(parseInstruction(userInput)) {
			case INST_SAVE:		if(boolPrompt("Save and exit?")) {brk = true;}			break;
			case INST_CANCEL:	if(boolPrompt("Cancel and exit?")) {brk = true; cancel = true;}	break;
			case INST_MAKE_NODE:	mknode(&names, &edges);						break;
			case INST_REMOVE_NODE:	rmnode(&names, &edges);						break;
			case INST_LIST_NODES:	printNames(names);						break;
			case INST_MAKE_EDGE:	mkedge(names, &edges);						break;
			case INST_REMOVE_EDGE:	rmedge(names, &edges);						break;
			case INST_LIST_EDGES:	printEdges(names, getEdges(edges));				break;
			case INST_INVALID:	print_help();							break;
		}
	}
	if(cancel) {return 0;}
	
	//data juggling
	const int sidelength = 2000;
	const int nodesize = 10;
	const int textsize = 50;

	int centerX = sidelength/2;
	int centerY = sidelength/2;
	int radius = (sidelength/2) - ((sidelength/2)/5);

	std::vector<pos> nodepositions = {};
	for(int i = 0; i < names.size(); i++) {
		double angle = 2 * M_PI * i / names.size();
		int nodeXpos = 0;
		int nodeYpos = 0;
		nodeXpos += round(centerX + radius * cos(angle));
		nodeYpos += round(centerY + radius * sin(angle));
		nodepositions.push_back({nodeXpos, nodeYpos});
	}

	std::vector<edge> edgeindexes = getEdges(edges);

	//image generation: prep
	cairo_surface_t *pngsurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, sidelength, sidelength);
	cairo_t *pngcontext = cairo_create(pngsurface);
	cairo_set_source_rgb(pngcontext, 0, 0, 0);
	cairo_paint(pngcontext);

	//image generation: edges
	cairo_save(pngcontext);
	cairo_set_line_width(pngcontext, 2);
	cairo_set_source_rgb(pngcontext, 1, 1, 1);
	for(int i = 0; i < edgeindexes.size(); i++) {
		int sourceX = nodepositions[edgeindexes[i].nodeA].xpos;
		int sourceY = nodepositions[edgeindexes[i].nodeA].ypos;
		int targetX = nodepositions[edgeindexes[i].nodeB].xpos;
		int targetY = nodepositions[edgeindexes[i].nodeB].ypos;
		cairo_move_to(pngcontext, sourceX, sourceY);
		cairo_line_to(pngcontext, targetX, targetY);
		cairo_stroke(pngcontext);
	}
	cairo_restore(pngcontext);

	//image generation: nodes
	cairo_save(pngcontext);
	cairo_set_source_rgb(pngcontext, 1, 1, 1);
	for(int i = 0; i < names.size(); i++) {
		cairo_arc(pngcontext, nodepositions[i].xpos, nodepositions[i].ypos, nodesize, 0, 2*M_PI);
		cairo_fill(pngcontext);
	}
	cairo_restore(pngcontext);

	//image generation: node rings
	cairo_save(pngcontext);
	cairo_set_source_rgb(pngcontext, 0, 0, 0);
	for(int i = 0; i < names.size(); i++) {
		cairo_arc(pngcontext, nodepositions[i].xpos, nodepositions[i].ypos, nodesize-2, 0, 2*M_PI);
		cairo_fill(pngcontext);
	}
	cairo_restore(pngcontext);

	//image generation: node text
	cairo_save(pngcontext);
	cairo_set_source_rgb(pngcontext, 1, 1, 1);
	cairo_select_font_face(pngcontext, "Monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(pngcontext, textsize);
	cairo_text_extents_t(textextents);
	for(int i = 0; i < names.size(); i++) {
		cairo_text_extents(pngcontext, names[i].c_str(), &textextents);

		cairo_move_to(
			pngcontext,
			nodepositions[i].xpos - (textextents.width/2 + textextents.x_bearing),
			nodepositions[i].ypos - (textextents.height/2 + textextents.y_bearing)
		);
		cairo_show_text(pngcontext, names[i].c_str());
	}
	cairo_restore(pngcontext);

	//image generation: cleanup
	cairo_destroy(pngcontext);

	//file stuff
	std::string pngname = argv[1];
	cairo_surface_write_to_png(pngsurface, pngname.c_str());
	cairo_surface_destroy(pngsurface);
	return 0;
}
