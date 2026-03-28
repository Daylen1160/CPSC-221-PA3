/**
 * @file tritree-private.h
 * @author dchun924
 * @description TriTree private function declarations, CPSC 221 PA3
 *
 * This file must be submitted for grading,
 * even if you do not add anything to it.
 *
 * Declare your functions here as if they are written in-line
 * starting from line 220 of tritree.h
**/


int CountLeaves(const Node* node) const;

void ClearNode(Node* node);

Node* CopyNode(const Node* node);

void RenderNode(const Node* node, PNG& out) const;

void TransposeNode(Node* node);
