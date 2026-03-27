/**
 * @file tritree.cpp
 * @author CPSC 221
 * @description TriTree class implementation, CPSC 221 PA3
 *
 * This file must be submitted for grading.
 *
**/

#include "tritree.h"

TriTree::TriTree(PNG& imIn) {
	// REPLACE THE LINEs BELOW WITH YOUR CODE
	width = 0;
	height = 0;
	root = nullptr;
}

void TriTree::Clear() {
	// YOUR CODE HERE
}

void TriTree::Copy(const TriTree& other) {
	// YOUR CODE HERE
}

PNG TriTree::Render() const {
	// REPLACE THE LINE BELOW WITH YOUR CODE
	return PNG();
}

void TriTree::Transpose() {
	// YOUR CODE HERE
}

void TriTree::Prune(double tol) {
	// YOUR CODE HERE
}

int TriTree::NumLeaves() const {
	// REPLACE THE LINE BELOW WITH YOUR CODE
	return 0;
}

Node* TriTree::BuildNode(PNG& im, pair<int, int> ul, int w, int h) {
	if (w == 1 && h == 1) {
		Node *leaf = new Node(ul, w, h);
		leaf->avg = *im.getPixel(ul.first, ul.second);
		return leaf;
	}

	Node *node = new Node(ul, w, h);
	int wA, hA;
	int wB, hB;
	int wC, hC;
	pair<int, int> ulA;
	pair<int, int> ulB;
	pair<int, int> ulC;

	if (w >= h) {
		if (w == 2) {
			ulA = {ul.first, ul.second};
			ulC = {ul.first + 1, ul.second};
			wA = 1;
			wC = 1;
			hA = h;
			hC = h;
			node->A = BuildNode(im, ulA, wA, hA);
			node->B = nullptr;
			node->C = BuildNode(im, ulC, wC, hC);
		}

		else if (w % 3 == 0) {
			wA = w / 3;
			wB = w / 3;
			wC = w / 3;
		}

		else if (w % 3 == 1) {
			wA = w / 3;
			wB = w / 3 + 1;
			wC = w / 3;
		}

		else {
			wA = w / 3 + 1;
			wB = w / 3;
			wC = w / 3 + 1;
		}

		hA = h;
		hB = h;
		hC = h;
		ulA = {ul.first, ul.second};
		ulB = {ul.first + wA, ul.second};
		ulC = {ul.first + wA + wB, ul.second};
	}

	else {
		if (h == 2) {
			ulA = {ul.first, ul.second};
			ulC = {ul.first, ul.second + 1};
			wA = w;
			wC = w;
			hA = 1;
			hC = 1;
			node->A = BuildNode(im, ulA, wA, hA);
			node->B = nullptr;
			node->C = BuildNode(im, ulC, wC, hC);
		}

		else if (h % 3 == 0) {
			hA = h / 3;
			hB = h / 3;
			hC = h / 3;
		}

		else if (h % 3 == 1) {
			hA = h / 3;
			hB = h / 3 + 1;
			hC = h / 3;
		}

		else {
			hA = h / 3 + 1;
			hB = h / 3;
			hC = h / 3 + 1;
		}

		wA = w;
		wB = w;
		wC = w;
		ulA = {ul.first, ul.second};
		ulB = {ul.first, ul.second + hA};
		ulC = {ul.first, ul.second + hA + hB};
	}
	
	node->A = BuildNode(im, ulA, wA, hA);
	node->B = BuildNode(im, ulB, wB, hB);
	node->C = BuildNode(im, ulC, wC, hC);
	
	uint64_t r = 0, g = 0, b = 0, a = 0;
	for (int x = ul.first; x < ul.first + w; x++) {
		for (int y = ul.second; y < ul.second + h; y++) {
			RGBAPixel* p = im.getPixel(x, y);
			r += p->r;
			g += p->g;
			b += p->b;
			a += p->a;
		}
	}
	int count = w * h;
	node->avg.r = r / count;
	node->avg.g = g / count;
	node->avg.b = b / count;
	node->avg.a = a / count;
	
	return node;
}

/*==== ALSO IMPLEMENT ANY PRIVATE FUNCTIONS YOU HAVE DECLARED ====*/