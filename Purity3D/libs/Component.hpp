#pragma once

class Component {
public:
	/*! \brief Lifecycle called when component is added to gameobject */
	virtual void onGetOtherComponent(Component* component) {}
	/*! \brief Lifecycle called when component is removed from gameobject*/
	virtual void onRemoveOtherComponent(Component component) {}
	/*! \brief Lifecycle called when component is added to parent */
	virtual void onGetParentComponent(Component* component) {}
	/*! \brief Lifecycle called when component is removed from parent */
	virtual void onRemoveParentComponent(Component* component) {}
	/*! \brief Method to get parent gameObject */
	virtual void getGameObject(Component* gameObject) {}
	/*! \brief Lifecycle called when game starts or component is added to game */
	virtual void onStart() {}
	/*! \brief Lifecycle called on updating frame before drawing */
	virtual void onUpdate() {}
	/*! \brief Lifecycle called on drawing on screen*/
	virtual void onDraw() {}
	/*! \brief Lifecycle called when object is removed */
	virtual void onDelete() {}
protected:
	/*! \brief Saves component if is instance of target to target */
	template<class T> void storeIfIsInstance(T** target, Component* component) {
		T* t = dynamic_cast<T*>(component);
		if (!t) {
			return;
		}
		(*target) = t;
	}
	/*! \brief Removes component if is instance of target from target */
	template<class T> void removeIfIsInstance(T** target, Component* component) {
		T* t = dynamic_cast<T*>(component);
		if (t) {
			(*target) = nullptr;
		}
	}
	/*! \brief Checks if component is instance of target */
	template<class T> bool isInstance(T** target, Component* component) {
		T* t = dynamic_cast<T*>(component);
		return t;
	}

};