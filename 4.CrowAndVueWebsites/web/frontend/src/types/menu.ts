export interface MenuItem {
  id: number;
  title: string;
  icon: string;
  isExpanded?: boolean;
  children?: SubMenuItem[];
}

export interface SubMenuItem {
  id: number;
  parentId: number;
  title: string;
  path: string;
  icon?: string;
  permission?: string[];
}

export interface MenuState {
  isCollapsed: boolean;
  activeMenuId: number | null;
  activeSubMenuId: number | null;
  menus: MenuItem[];
}
