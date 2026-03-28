/**
 * @file tritree.cpp
 * @author dchun924
 * @description TriTree class implementation, CPSC 221 PA3
 *
 * This file must be submitted for grading.
 *
**/

#include "tritree.h"

#include <cstdint>

TriTree::TriTree(PNG& imIn) {
	if (imIn.width() == 0 || imIn.height() == 0) {
		width = 0;
		height = 0;
		root = nullptr;
		return;
	}
	width = imIn.width();
	height = imIn.height();
	root = BuildNode(imIn, {0, 0}, width, height);
}

void TriTree::Clear() {
	ClearNode(root);
	root = nullptr;
	width = 0;
	height = 0;
}

void TriTree::Copy(const TriTree& other) {
	width = other.width;
	height = other.height;
	root = CopyNode(other.root);
}

PNG TriTree::Render() const {
	PNG myPNG = PNG(width, height);
	RenderNode(root, myPNG);
	return myPNG;
}

void TriTree::Transpose() {
	TransposeNode(root);
	int temp = width;
	width = height;
	height = temp;
}

void TriTree::Prune(double tol) {
	Pruner(root, tol);
}

void TriTree::Pruner(Node*& node, double tol) {
    if (node == nullptr) {
        return;
    }

    if (WithinTol(node, node->avg, tol)) {
        ClearNode(node->A);
        ClearNode(node->B);
        ClearNode(node->C);
    } else {
        Pruner(node->A, tol);
        Pruner(node->B, tol);
        Pruner(node->C, tol);
    }
}

bool TriTree::WithinTol(Node* curr, RGBAPixel& base, double tol) const {
    if (curr == nullptr) {
        return true;
    }

    if (curr->A == nullptr && curr->B == nullptr && curr->C == nullptr) {
        return curr->avg.dist(base) <= tol;
    }

    return WithinTol(curr->A, base, tol) && WithinTol(curr->B, base, tol) && WithinTol(curr->C, base, tol);
}

int TriTree::NumLeaves() const {
	return CountLeaves(root);
}

Node* TriTree::BuildNode(PNG& im, pair<int, int> ul, int w, int h) {
    if (w <= 0 || h <= 0) {
        return nullptr;
    }

    Node* node = new Node(ul, w, h);

	if (w == 1 && h == 1) {
		node->avg = *im.getPixel(ul.first, ul.second);
		return node;
	}

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
        else {
            if (w % 3 == 0) {
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

            node->A = BuildNode(im, ulA, wA, hA);
            node->B = BuildNode(im, ulB, wB, hB);
            node->C = BuildNode(im, ulC, wC, hC);
        }
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
        else {
            if (h % 3 == 0) {
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

            node->A = BuildNode(im, ulA, wA, hA);
            node->B = BuildNode(im, ulB, wB, hB);
            node->C = BuildNode(im, ulC, wC, hC);
        }
	}
	
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

int TriTree::CountLeaves(const Node *node) const {
	if (node == nullptr) return 0;
	if (node->A == nullptr && node->B == nullptr && node->C == nullptr) return 1;
	return CountLeaves(node->A) + CountLeaves(node->B) + CountLeaves(node->C);
}

void TriTree::ClearNode(Node*& node) {
	if (node == nullptr) return;
	ClearNode(node->A);
	ClearNode(node->B);
	ClearNode(node->C);

	delete node;
    node = nullptr;
}

Node* TriTree::CopyNode(const Node* node) {
	if (node == nullptr) return nullptr;

	Node* copy = new Node(node->upperleft, node->width, node->height);
	copy->avg = node->avg;
	copy->A = CopyNode(node->A);
	copy->B = CopyNode(node->B);
	copy->C = CopyNode(node->C);
	return copy;
}

void TriTree::RenderNode(const Node* node, PNG& out) const {
	if (node == nullptr) return;

	if (node->A == nullptr && node->B == nullptr && node->C == nullptr) {
		int startX = node->upperleft.first;
		int startY = node->upperleft.second;
		for (int x = startX; x < startX + node->width; x++) {
			for (int y = startY; y < startY + node->height; y++) {
				*out.getPixel(x, y) = node->avg;
			}
		}
		return;
	}

	RenderNode(node->A, out);
	RenderNode(node->B, out);
	RenderNode(node->C, out);
}

void TriTree::TransposeNode(Node* node) {
	if (node == nullptr) return;

	int tempCoord = node->upperleft.first;
	node->upperleft.first = node->upperleft.second;
	node->upperleft.second = tempCoord;

	int tempDim = node->width;
	node->width = node->height;
	node->height = tempDim;

	TransposeNode(node->A);
	TransposeNode(node->B);
	TransposeNode(node->C);
}