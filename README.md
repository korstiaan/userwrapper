User Wrapper for Drupal 7.x
========================
Drupal module which provides simple access to a user's fields.


Requirements
--------------------------------

* Drupal 7.x
* PHP 5.3.2 or higher
* Namespace Autoload (https://github.com/korstiaan/nsautoload)
* Entity Base (https://github.com/korstiaan/entitybase)


Setup / Initial Installation
--------------------------------

Install it as a normal Drupal module. This means downloading (or git clone'ing) it to site/all/modules and enabling it on "admin/modules/list".

If you're using Voiture (http://voiture.hoppinger.com) just add "userwrapper" to cnf/shared/modules.php


Usage
--------------------------------

* Initializing:

$foo = new UserWrapper\User($user);

$foo = UserWrapper\User::createByUid($user->uid);

* get/set field value:

$value = $foo->get('field_bar');

$foo->set('field_crux','lipsum');

$foo->set('pass','mysupersecretpassword');

Look at the inline (phpdoc) documentation for more info.
 