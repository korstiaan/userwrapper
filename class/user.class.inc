<?php
/**
 *	User Wrapper
 *  Copyright (C) 2011  Korstiaan de Ridder <korstiaan [at] korstiaan.com>
 *
 *	This file is part of User Wrapper.
 *
 *  User Wrapper is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  User Wrapper is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with User Wrapper.  If not, see <http://www.gnu.org/licenses/>.
 */
namespace UserWrapper;
use UserWrapper\User;
use EntityBase\EntityBase;

class User extends EntityBase implements UserInterface
{
	const		TYPE	= 'user';

	private	$base = array(
		'name','pass','mail','theme','signature','created','status','timezone','language','picture','data','roles'
	);
		
	/**
	 * 
	 * Save the node to db
	 * 
	 * @see 		NodeWrapper.NodeInterface::save()
	 * @see 		node_save()
	 * 
	 * @return		void
	 */
	public function save()
	{
		user_save($this->getEntity());
	}
	
	/**
	 * 
	 * Does the normal field set magic except for:
	 *  - pass : hashes the password initially  
	 * 
	 * @see EntityBase.EntityBase::set()
	 */
	public function set($field, $val)
	{
		switch ($field) {
			case 'password':
				$field = 'pass';
			case 'pass':
				if (strlen($val) === 0) {
					throw new \InvalidArgumentException('Trying to set an empty password');
				}
				require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
				$val = user_hash_password($val);
				break;
		}
		parent::set($field, $val);		
	}
	
	/**
	 * @see	 	EntityBase.EntityInterface::getEntityType() 
	 * @return	string
	 */
	public function getEntityType() { return self::TYPE; }
	
	/**
	 * 
	 * Returns the id of the user. 
	 * 
	 * @see 	EntityBase.EntityInterface::getId() 
	 * @return	integer|boolean			uid if set, false otherwise
	 */
	public function getId()
	{
		return isset($this->getEntity()->uid) ? (int)$this->getEntity()->uid : false;
	}
	
	/**
	 * 
	 * Returns true if user has given role id 
	 * 
	 * @see 	UserWrapper.UserInterface::hasRole()
	 * 
	 * @param	integer		$role	
	 * @return	boolean
	 */
	public function hasRole($role)
	{
		$roles = $this->get('roles');
		return is_array($roles) && isset($roles[$role]);
	}
	
	/**
	 * 
	 * Returns true if user has anonymous role
	 * 
	 * @see 		UserWrapper.UserInterface::isAnonymous() 
	 * @return		boolean
	 */
	public function isAnonymous()
	{
		return $this->hasRole(DRUPAL_ANONYMOUS_RID);
	}
		
	/**
	 * @see EntityBase.EntityBase::getBaseFields()
	 */
	protected function getBaseFields()
	{
		return (array)$this->base;
	}
	
	/**
	 * 
	 * Bundle name for a user is always "user"
	 * 
	 * @see 	EntityBase.EntityBase::getBundleName() 
	 * @return	string
	 */
	protected function getBundleName() { return 'user'; }
	
	/**
	 * 
	 * Returns User Wrapper for node of given id
	 * 
	 * @param 	integer 	$nid
	 * @throws 	\InvalidArgumentException
	 * @return	User
	 */
	static function createByUid($uid) 
	{
		if (!is_numeric($uid)) {
			throw new \InvalidArgumentException('Param should be numeric');
		}
		static $ret = array();
		if (!isset($ret[$uid])) {
			$ret[$uid] = new static(user_load($uid));
		}
		return $ret[$uid];
	}
}