--creation roles
CREATE ROLE group_hr;
CREATE ROLE group_pd;
CREATE ROLE group_controller;
CREATE ROLE group_admin WITH CREATEROLE CREATEDB;
GRANT CONNECT ON DATABASE acms TO group_hr, group_pd, group_controller, group_admin;
GRANT USAGE ON SCHEMA public TO group_hr, group_pd, group_controller, group_admin;
--hr
GRANT SELECT ON TABLE public.staff, public.departments, public.access_controller, public.access_facts, public.pass TO group_hr;
GRANT INSERT, UPDATE, DELETE ON TABLE public.departments TO group_hr;
GRANT INSERT, UPDATE, DELETE ON TABLE public.staff TO group_hr;
GRANT DELETE ON TABLE public.pass TO group_hr;
GRANT DELETE ON TABLE public.purpose TO group_hr;
GRANT DELETE ON TABLE public.access_facts TO group_hr;
GRANT USAGE, SELECT ON SEQUENCE public.departments_department_id_seq TO group_hr;
GRANT USAGE, SELECT ON SEQUENCE public.staff_staff_id_seq TO group_hr;
--Controller
GRANT INSERT ON TABLE public.access_facts TO group_controller;
GRANT SELECT ON TABLE public.access_controller, public.pass, public.staff, public.purpose, public.access_groups, public.group_consists TO group_controller;
--PD
GRANT SELECT ON TABLE public.staff, public.departments, public.pass, public.access_groups, public.group_consists, public.access_controller, public.purpose TO group_pd;
GRANT INSERT ON TABLE public.access_groups, public.pass, public.group_consists, public.purpose TO group_pd;
GRANT UPDATE ON TABLE public.access_groups, public.pass TO group_pd;
GRANT DELETE ON TABLE public.access_groups, public.group_consists, public.pass, public.purpose TO group_pd;
GRANT SELECT ON TABLE public.access_facts TO group_pd;
GRANT DELETE ON TABLE public.access_facts TO group_pd;
GRANT USAGE, SELECT ON SEQUENCE public.access_groups_group_id_seq TO group_pd;
GRANT USAGE, SELECT ON SEQUENCE public.pass_pass_id_seq TO group_pd;
GRANT USAGE, SELECT ON SEQUENCE public.group_consists_position_id_seq TO group_pd;
GRANT USAGE, SELECT ON SEQUENCE public.purpose_purpose_id_seq TO group_pd;
--Admin
GRANT ALL PRIVILEGES ON SCHEMA public TO group_admin WITH GRANT OPTION;
GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO group_admin WITH GRANT OPTION;
GRANT ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public TO group_admin WITH GRANT OPTION;
GRANT ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public TO group_admin WITH GRANT OPTION;
GRANT CREATE ON SCHEMA public TO group_admin;
GRANT CREATE ON DATABASE acms TO group_admin;
GRANT group_controller TO group_admin WITH ADMIN OPTION;
GRANT group_hr TO group_admin WITH ADMIN OPTION;
GRANT group_pd TO group_admin WITH ADMIN OPTION;
--creation first admin
CREATE USER acms_admin WITH PASSWORD 'CV';
GRANT group_admin TO acms_admin WITH ADMIN OPTION;
INSERT INTO users (login, passphrase, user_role) VALUES ('acms_admin', 'CV', 'Admin');