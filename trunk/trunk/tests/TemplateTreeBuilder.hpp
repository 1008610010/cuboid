#ifndef XTL__TEMPLATE_TREE_BUILDER_HPP__
#define XTL__TEMPLATE_TREE_BUILDER_HPP__ 1

#include <stdio.h>

#include <stdexcept>
#include <string>

#include "../lib/xtl/utils/AutoPtrVector.hpp"

namespace XTL
{
	class TemplateNode;

	class TemplateNodeList
	{
		public:

			explicit TemplateNodeList(TemplateNode * parentNode)
				: parentNode_(parentNode),
				  items_()
			{
				;;
			}

			TemplateNode * GetParentNode() const
			{
				return parentNode_;
			}

			bool IsEmpty() const
			{
				return items_.IsEmpty();
			}

			TemplateNode * LastNode()
			{
				return items_.IsEmpty() ? 0 : items_.Back();
			}

			void Add(std::auto_ptr<TemplateNode> node)
			{
				items_.PushBack(node);
			}

			void DebugPrint(unsigned int indent) const;

		private:

			TemplateNode * const parentNode_;
			AutoPtrVector<TemplateNode> items_;
	};

	class TemplateNode
	{
		public:

			enum Type
			{
				TEXT,
				VARIABLE,
				BRANCHING,
				LOOP
			};

			TemplateNode(TemplateNodeList * parentList)
				: parentList_(parentList)
			{
				;;
			}

			virtual ~TemplateNode() throw() { ;; }

			TemplateNodeList * GetParentList() const
			{
				return parentList_;
			}

			virtual Type GetType() const = 0;

			virtual void DebugPrint(unsigned int indent) const = 0;

			class Text;
			class Variable;
			class Branching;
			class Loop;

		protected:

			static void DebugPrintIndent(unsigned int indent)
			{
				for (; indent > 0; --indent)
				{
					printf("    ");
				}
			}

			static void DebugPrintIndent(unsigned int indent, const char * s)
			{
				DebugPrintIndent(indent);
				printf("%s", s);
			}

		private:

			TemplateNodeList * const parentList_;
	};

	class TemplateNode::Text : public TemplateNode
	{
		public:

			Text(TemplateNodeList * parentList, const std::string & text)
				: TemplateNode(parentList),
				  text_(text)
			{
				;;
			}

			virtual ~Text() throw()
			{
				;;
			}

			virtual Type GetType() const
			{
				return TEXT;
			}

			virtual void DebugPrint(unsigned int indent) const
			{
				DebugPrintIndent(indent);
				printf("TEXT\n");
			}

			void AppendText(const std::string & text)
			{
				text_.append(text);
			}

		private:

			std::string text_;
	};

	class TemplateNode::Variable : public TemplateNode
	{
		public:

			Variable(TemplateNodeList * parentList, const std::string & name)
				: TemplateNode(parentList),
				  name_(name)
			{
				;;
			}

			virtual ~Variable() throw() { ;; }

			virtual Type GetType() const
			{
				return VARIABLE;
			}

			virtual void DebugPrint(unsigned int indent) const
			{
				DebugPrintIndent(indent, "VARIABLE\n");
			}

		private:

			const std::string name_;
	};

	class TemplateNode::Branching : public TemplateNode
	{
		public:

			class Branch
			{
				public:

					Branch(TemplateNode * parentNode, const std::string & varName)
						: varName_(varName),
						  list_(parentNode)
					{
						;;
					}

					const TemplateNodeList * GetList() const
					{
						return &list_;
					}

					TemplateNodeList * GetList()
					{
						return &list_;
					}

				private:

					const std::string varName_;
					TemplateNodeList  list_;
			};

			explicit Branching(TemplateNodeList * parentList)
				: TemplateNode(parentList),
				  branches_(),
				  branchElse_()
			{
				;;
			}

			virtual ~Branching() throw() { ;; }

			virtual Type GetType() const
			{
				return BRANCHING;
			}

			virtual void DebugPrint(unsigned int indent) const
			{
				for (unsigned int i = 0; i < branches_.Size(); ++i)
				{
					if (i == 0)
					{
						DebugPrintIndent(indent, "IF {\n");
					}
					else
					{
						DebugPrintIndent(indent, "}\n");
						DebugPrintIndent(indent, "ELSEIF {\n");
					}
					branches_[i]->GetList()->DebugPrint(indent + 1);
				}
				if (branchElse_.get() != 0)
				{
					DebugPrintIndent(indent, "}\n");
					DebugPrintIndent(indent, "ELSE {\n");
					branchElse_->DebugPrint(indent + 1);
				}
				DebugPrintIndent(indent, "}\n");
			}

			TemplateNodeList * AddBranch(const std::string & varName)
			{
				std::auto_ptr<Branch> newBranch(new Branch(this, varName));
				TemplateNodeList * branchList = newBranch->GetList();
				branches_.PushBack(newBranch);
				return branchList;
			}

			TemplateNodeList * AddBranchElse()
			{
				branchElse_.reset(new TemplateNodeList(this));
				return branchElse_.get();
			}

			bool HasBranchElse() const
			{
				return branchElse_.get() != 0;
			}

		private:

			AutoPtrVector<Branch> branches_;
			std::auto_ptr<TemplateNodeList> branchElse_;
	};

	class TemplateNode::Loop : public TemplateNode
	{
		public:

			Loop(TemplateNodeList * parentList)
				: TemplateNode(parentList),
				  body_(this)
			{
				;;
			}

			TemplateNodeList * GetBody()
			{
				return &body_;
			}

			virtual Type GetType() const
			{
				return LOOP;
			}

			virtual void DebugPrint(unsigned int indent) const
			{
				DebugPrintIndent(indent, "LOOP {\n");
				body_.DebugPrint(indent + 1);
				DebugPrintIndent(indent, "}\n");
			}

		private:

			TemplateNodeList body_;
	};

	class TemplateTreeBuilder
	{
		public:

			TemplateTreeBuilder()
				: rootList_(new TemplateNodeList(0)),
				  currentList_(rootList_.get())
			{
				;;
			}

			void AddText(const std::string & text)
			{
				if (text.size() > 0)
				{
					printf("TEXT: length=%u\n", (unsigned int) text.size());

					TemplateNode * lastNode = currentList_->LastNode();
					if (lastNode == 0 || lastNode->GetType() != TemplateNode::TEXT)
					{
						std::auto_ptr<TemplateNode> newNode(new TemplateNode::Text(currentList_, text));
						currentList_->Add(newNode);
					}
					else
					{
						static_cast<TemplateNode::Text *>(lastNode)->AppendText(text);
					}

					/*
					std::auto_ptr<TemplateNode> newNode(new TemplateNode::Text(currentList_, text));
					currentList_->Add(newNode);
					*/
				}
			}

			void OpenIf(const std::string & varName)
			{
				printf("{{ if %s }}\n", varName.c_str());

				TemplateNode::Branching * newNode = new TemplateNode::Branching(currentList_);
				currentList_->Add(std::auto_ptr<TemplateNode>(newNode));
				currentList_ = newNode->AddBranch(varName);
			}

			void OpenElseIf(const std::string & varName)
			{
				printf("{{ elseif %s }}\n", varName.c_str());

				TemplateNode * parentNode = currentList_->GetParentNode();
				if (parentNode == 0 || parentNode->GetType() != TemplateNode::BRANCHING)
				{
					throw std::runtime_error("Syntax error");
				}

				TemplateNode::Branching * branching = static_cast<TemplateNode::Branching *>(parentNode);
				if (branching->HasBranchElse())
				{
					throw std::runtime_error("Syntax error");
				}

				currentList_ = branching->AddBranch(varName);
			}

			void OpenElse()
			{
				printf("{{ else }}\n");

				TemplateNode * parentNode = currentList_->GetParentNode();
				if (parentNode == 0 || parentNode->GetType() != TemplateNode::BRANCHING)
				{
					throw std::runtime_error("Syntax error");
				}

				TemplateNode::Branching * branching = static_cast<TemplateNode::Branching *>(parentNode);
				if (branching->HasBranchElse())
				{
					throw std::runtime_error("Syntax error");
				}

				currentList_ = branching->AddBranchElse();
			}

			void CloseIf()
			{
				printf("{{ /if }}\n");

				TemplateNode * parentNode = currentList_->GetParentNode();
				if (parentNode == 0 || parentNode->GetType() != TemplateNode::BRANCHING)
				{
					throw std::runtime_error("Syntax error");
				}

				currentList_ = parentNode->GetParentList();
			}

			void OpenLoop(const std::string & varName)
			{
				printf("{{ loop %s }}\n", varName.c_str());

				TemplateNode::Loop * newNode = new TemplateNode::Loop(currentList_);
				currentList_->Add(std::auto_ptr<TemplateNode>(newNode));
				currentList_ = newNode->GetBody();
			}

			void CloseLoop()
			{
				printf("{{ /loop }}\n");

				TemplateNode * parentNode = currentList_->GetParentNode();
				if (parentNode == 0 || parentNode->GetType() != TemplateNode::LOOP)
				{
					throw std::runtime_error("Syntax error");
				}

				currentList_ = parentNode->GetParentList();
			}

			std::auto_ptr<TemplateNodeList> Release()
			{
				if (currentList_ != rootList_.get())
				{
					throw std::runtime_error("Syntax error");
				}

				return rootList_;
			}

		private:

			std::auto_ptr<TemplateNodeList> rootList_;
			TemplateNodeList * currentList_;
	};
}

#endif
